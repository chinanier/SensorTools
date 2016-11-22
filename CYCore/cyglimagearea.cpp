#include "CYGLImageArea.h"
#include "QOpenGLTexture"
#include "QMatrix"
#include "QMatrix4x4"
#include "QOpenGLBuffer"
#include <QOpenGLShaderProgram>
#include <QOpenGLPixelTransferOptions>
#include <QMessageBox>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QDebug>

#include <qt_windows.h>

#define  TEST_TEX (1)

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};
class CYGLImageAreaPrivate
{
public:
    CYGLImageAreaPrivate(CYGLImageArea * q) :
        q_ptr(q),
        texture(0),
        indexBuf(QOpenGLBuffer::IndexBuffer),
        texBuf(QOpenGLBuffer::PixelUnpackBuffer),
        textype(0),
        scaleRatio(1.20f),
        zoomValue(100.0f)
    {
        width = 0;
        height = 0;
        translatePos.setX(0);
        translatePos.setY(0);
    }
    ~CYGLImageAreaPrivate()
    {
        delete texture;
    }
public:
    void initializeModle()    // - 初始化模型
    {
        // 模型的世界坐标与纹理大小相同,方便处理
        // 
    }
    void initShaders()
    {
        QString strfshader = "#ifdef GL_ES\n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif\n"
            "\n"
            "uniform sampler2D texture;\n"
            "uniform int textype;\n"
            "varying vec2 v_texcoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // Set fragment color from texture\n"
            "    vec3 rgb;\n"
            "    if (textype == 0)\n"
            "    {\n"
            "        rgb.r = texture2D(texture, v_texcoord).r;\n"
            "        rgb.g = rgb.r;\n"
            "        rgb.b = rgb.r;\n"
            "        gl_FragColor = vec4(rgb, 1);\n"
            "    }\n"
            "    else\n"
            "    {\n"
            "        gl_FragColor = texture2D(texture, v_texcoord);\n"
            "    }\n"
            "    /*gl_FragColor = texture2D(texture, v_texcoord)vec4(1.0,0.0,1.0,1.0)*/;\n"
            "    //gl_FragColor = vec4(rgb,1);\n"
            "}\n";
        QString strvshader = "#ifdef GL_ES\n"
            "// Set default precision to medium\n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif\n"
            "\n"
            "uniform mat4 mvp_matrix;\n"
            "\n"
            "attribute vec4 a_position;\n"
            "attribute vec2 a_texcoord;\n"
            "\n"
            "varying vec2 v_texcoord;\n"
            "\n"
            "//! [0]\n"
            "void main()\n"
            "{\n"
            "    // Calculate vertex position in screen space\n"
            "    gl_Position = mvp_matrix * a_position;\n"
            "\n"
            "    // Pass texture coordinate to fragment shader\n"
            "    // Value will be automatically interpolated to fragments inside polygon faces\n"
            "    v_texcoord = a_texcoord;\n"
            "}\n"
            "//! [0]\n";
        // Compile vertex shader
        if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, strvshader))
        {
            QString strinfo;
            QMessageBox::information(0, "OpenGL initShaders", program.log());
            return;
        }
        
        // Compile fragment shader
        if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, strfshader))
        {
            QString strinfo;
            QMessageBox::information(0, "OpenGL initShaders", program.log());
            return;
        }

        // Link shader pipeline
        if (!program.link())
        {
            QString strinfo;
            QMessageBox::information(0, "OpenGL initShaders", program.log());
            return;
        }

        // Bind shader pipeline for use
        if (!program.bind())
        {
            QString strinfo;
            QMessageBox::information(0, "OpenGL initShaders", program.log());
            return;
        }
    }
    void initTextures(int width, int height, uint sourceFormat, uint sourceType)
    {
#if TEST_TEX
        return;
#else
        // 重置
        if (texture)
        {
            //q_ptr->makeCurrent();
            //delete texture;
            //texture = 0;
            //q_ptr->doneCurrent();
            texture->destroy();
        }
        else
        {
            texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        }
        QOpenGLContext *context = QOpenGLContext::currentContext();
        if (!context) {
            qWarning("QOpenGLTexture::setData() requires a valid current context");
            return;
        }

        if (sourceFormat != QOpenGLTexture::Luminance)
        {
            if (context->isOpenGLES() && context->format().majorVersion() < 3)
                texture->setFormat(QOpenGLTexture::RGBAFormat);
            else
                texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
        }
        else
        {
            texture->setFormat(QOpenGLTexture::LuminanceFormat);
        }

        texture->setSize(width, height);
        texture->setMipLevels(texture->maximumMipLevels());
        texture->allocateStorage((QOpenGLTexture::PixelFormat)sourceFormat, (QOpenGLTexture::PixelType)sourceType);

        texBuf.destroy();
        texBuf.create();
        texBuf.bind();
        //texBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        texBuf.allocate(width * height * 4);
#endif
    }

    void initTexGeometry(int width, int height)
    {
        arrayBuf.destroy();
        indexBuf.destroy();

        arrayBuf.create();
        indexBuf.create();

        float halfwidth = width / 2;
        float halfheight = height / 2;

        VertexData vertices[] = {
            // Vertex data for face 0
            { QVector3D(-halfwidth, -halfheight, 5.0f), QVector2D(0.0f, 1.0f) },  // v0
            { QVector3D(halfwidth, -halfheight, 5.0f), QVector2D(1.0f, 1.0f) },  // v1
            { QVector3D(-halfwidth,  halfheight, 5.0f), QVector2D(0.0f, 0.0f) },  // v2
            { QVector3D(halfwidth,  halfheight, 5.0f), QVector2D(1.0f, 0.0f) }   // v3
        };

        GLushort indices[] = {
            0,  1,  2,  3,  3     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        };

        // Transfer vertex data to VBO 0
        arrayBuf.bind();
        arrayBuf.allocate(vertices, 4 * sizeof(VertexData));

        // Transfer index data to VBO 1
        indexBuf.bind();
        indexBuf.allocate(indices, 4 * sizeof(GLushort));
    }

    void Draw()
    {
        // Modle-Project
        // Clear color and depth buffer
        q_ptr->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!texture)
        {
            return;
        }
        //texture->bind();
        q_ptr->glActiveTexture(GL_TEXTURE0);                                                                // 激活纹理单元
        q_ptr->glBindTexture(GL_TEXTURE_2D, texture->textureId());                                   // 使用纹理单元
                                                                                                     //! [6]
                                                                                                     // Calculate model view transformation
        QMatrix4x4 matrix;
        matrix.translate(translatePos.x(), translatePos.y(), -100.0);
        matrix.scale(zoomValue / 100.0f, zoomValue / 100.0f);
        //matrix.rotate(rotation);

        // Set modelview-projection matrix
        program.setUniformValue("mvp_matrix", projection * matrix);
        //! [6]

        // Use texture unit 0 which contains cube.png
        program.setUniformValue("texture", 0);
        //textype
        //program.setUniformValue("textype", 1);
        program.setUniformValue("textype", textype);
        // Tell OpenGL which VBOs to use
        arrayBuf.bind();
        indexBuf.bind();

        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program.attributeLocation("a_position");
        program.enableAttributeArray(vertexLocation);
        program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program.attributeLocation("a_texcoord");
        program.enableAttributeArray(texcoordLocation);
        program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        //glDrawArrays(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
        q_ptr->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);

        qDebug() << "Draw......";
    }
private:
    CYGLImageArea * q_ptr;
    int width;
    int height;
    QOpenGLShaderProgram program;      // 着色器(顶点、片元)
    QOpenGLBuffer arrayBuf;            // 顶点数据
    QOpenGLBuffer indexBuf;            // 索引数据
    QOpenGLBuffer texBuf;              // 纹理数据
    QOpenGLTexture *texture;           // 纹理
    int            textype;            // 纹理类型
    QMatrix4x4 projection;             // 透视投影矩阵
    float      scaleRatio;             // 缩放系数
    float      zoomValue;              // 当前缩放倍率
    QPointF    oldPoint;               // 记录鼠标按下时的坐标点
    QPointF    translatePos;
    friend class CYGLImageArea;
};

CYGLImageArea::CYGLImageArea(QWidget *parent)
    : QOpenGLWidget(parent),
    d_ptr(new CYGLImageAreaPrivate(this))
{

}

CYGLImageArea::~CYGLImageArea()
{
    if (d_ptr)
    {
        delete d_ptr;
        d_ptr = 0;
    }
}


// QT OVERRIDE
void CYGLImageArea::mousePressEvent(QMouseEvent *e)
{
    // 标记为移动物体状态
    d_ptr->oldPoint = e->pos();
}
void CYGLImageArea::mouseMoveEvent(QMouseEvent *event)
{
    // 如果是移动状态,
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        qDebug() << "dksjahahsdfdshfjksdhf" << event->pos();
        // 根据当前的缩放比例,控制移动的范围
        // 1、计算当前缩放后,纹理的大小
        float texWidth = d_ptr->width*d_ptr->zoomValue / 100;
        float texHeight = d_ptr->height*d_ptr->zoomValue / 100;
        float viewWidth = width();
        float viewHeight = height();
        float widthMax = (texWidth - viewWidth) / 2 + 10;
        float heightMax = (texHeight - viewHeight) / 2 + 10;
        float cur_pos = 0.1f;

        if (widthMax > 0)
        {
            cur_pos = d_ptr->translatePos.x();
            cur_pos += event->pos().x() - d_ptr->oldPoint.x();
            cur_pos = cur_pos >= widthMax ? widthMax : (cur_pos <= -widthMax ? -widthMax : cur_pos);
            d_ptr->translatePos.setX(cur_pos);
        }

        if (heightMax > 0)
        {
            cur_pos = d_ptr->translatePos.y();
            cur_pos -= event->pos().y() - d_ptr->oldPoint.y();
            cur_pos = cur_pos >= heightMax ? heightMax : (cur_pos <= -heightMax ? -heightMax : cur_pos);
            d_ptr->translatePos.setY(cur_pos);
        }

        d_ptr->oldPoint = event->pos();
        update();
    }
}
void CYGLImageArea::mouseReleaseEvent(QMouseEvent *e)
{
    // 标记为取消移动物体状态
}
void CYGLImageArea::wheelEvent(QWheelEvent *e)
{
    qDebug() << e->delta();
    if (e->delta() > 0)
    {
        float val = qRound(d_ptr->zoomValue*1.2)*1.0f;
        float val2 = qRound(val * 1.2) * 1.0f;
        if (val < 100.0f &&val2 > 100.0f)
        {
            d_ptr->zoomValue = 100.0f;
        }
        else
        {
            if (val >= 1600)
            {
                d_ptr->zoomValue = 1600.0f;
            }
            else
            {
                d_ptr->zoomValue = val*1.0f;
            }
        }
    }
    else
    {
        float val = qRound(d_ptr->zoomValue / 1.2) * 1.0f;
        float val2 = qRound(val / 1.2) * 1.0f;
        if (val > 100.0f &&val2 < 100.0f)
        {
            d_ptr->zoomValue = 100.0f;
        }
        else
        {
            if (val < 5.0f)
            {
                d_ptr->zoomValue = 5.0f;
            }
            else
            {
                d_ptr->zoomValue = val*1.0f;
            }
        }
    }
    // 判断当前视口与纹理的关系,居中纹理显示
    float texWidth = d_ptr->width*d_ptr->zoomValue / 100;
    float texHeight = d_ptr->height*d_ptr->zoomValue / 100;
    float viewWidth = width();
    float viewHeight = height();
    float widthMax = (texWidth - viewWidth) / 2 + 10;
    float heightMax = (texHeight - viewHeight) / 2 + 10;
    float cur_pos = 0.1f;

    if (widthMax <= 0)
    {
        d_ptr->translatePos.setX(0.0f);
    }
    else
    {
        cur_pos = d_ptr->translatePos.x();
        cur_pos = cur_pos >= widthMax ? widthMax : (cur_pos <= -widthMax ? -widthMax : cur_pos);
        d_ptr->translatePos.setX(cur_pos);
    }

    if (heightMax <= 0)
    {
        d_ptr->translatePos.setY(0.0f);
    }
    else
    {
        cur_pos = d_ptr->translatePos.y();
        cur_pos = cur_pos >= heightMax ? heightMax : (cur_pos <= -heightMax ? -heightMax : cur_pos);
        d_ptr->translatePos.setY(cur_pos);
    }

    update();
}
void CYGLImageArea::timerEvent(QTimerEvent *e)
{

}
// GL OVERRIDE
void CYGLImageArea::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.5, 0.5, 0.5, 1);

    d_ptr->initShaders();

    //d_ptr->initTexGeometry(0, 0);

    //glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
#if TEST_TEX
    d_ptr->texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    d_ptr->texture->create();
#endif
    d_ptr->initializeModle();

    emit initialed();
}
void CYGLImageArea::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 2.0, zFar = 1000.0, fov = 45.0;

    // Reset projection
    d_ptr->projection.setToIdentity();

    // Set perspective projection
    //d_ptr->projection.perspective(fov, aspect, zNear, zFar);     // 创建透视投影矩阵
    d_ptr->projection.ortho(-w / 2, w / 2, -h / 2, h / 2, -1000, 1000);    // 创建正交投影变换矩阵
}
void CYGLImageArea::paintGL()
{
    DWORD t_start = GetTickCount();
    d_ptr->Draw();
    DWORD t_stop = GetTickCount();
    qDebug() << "paintGL:Using time : " << (t_stop - t_start) / 1000.0;
}

// 图像移动操作
bool CYGLImageArea::ZoomPlus(const bool bOn_i)
{
    return false;
}
bool CYGLImageArea::ZoomMin(const bool bOn_i)
{
    return false;
}
bool CYGLImageArea::ZoomOneToOne()
{
    return false;
}
bool CYGLImageArea::MakeToCenter()
{
    return false;
}

void CYGLImageArea::setData(int width, int height, uint sourceFormat, uint sourceType, const void *data)
{
    if (!isValid())
    {
        return;
    }
    if (!data)
    {
        return;
    }
#if !TEST_TEX
    if ((width != d_ptr->width) || (height != d_ptr->height))
    {
        d_ptr->initTextures(width, height, sourceFormat, sourceType);
        d_ptr->initTexGeometry(width, height);
        d_ptr->width = width;
        d_ptr->height = height;
    }
    QOpenGLPixelTransferOptions uploadOptions;
    uploadOptions.setAlignment(1);

    int ibites = (sourceFormat == QOpenGLTexture::Luminance) ? 1 : 4;
    d_ptr->texBuf.bind();
    d_ptr->texBuf.write(0, data, width * height*ibites);


    DWORD t_start = GetTickCount();
    d_ptr->texture->setData(0,
        /*QOpenGLTexture::RGBA*/
        (QOpenGLTexture::PixelFormat)sourceFormat,
        /*QOpenGLTexture::UInt8*/
        (QOpenGLTexture::PixelType)sourceType, /*data*/(const void*)0, &uploadOptions);
    DWORD t_stop = GetTickCount();
    qDebug() << "SetData:Using time : " << (t_stop - t_start) / 1000.0;
#else
    DWORD t_start = GetTickCount();
    if ((width != d_ptr->width) || (height != d_ptr->height))
    {
        //d_ptr->initTextures(width, height, sourceFormat, sourceType);
        d_ptr->initTexGeometry(width, height);
        d_ptr->width = width;
        d_ptr->height = height;
    }
    glActiveTexture(GL_TEXTURE0);                                                                // 激活纹理单元
    glBindTexture(GL_TEXTURE_2D, d_ptr->texture->textureId());                                   // 使用纹理单元
    if (sourceFormat == QOpenGLTexture::Luminance)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);    // 创建真正的数据
                                                                                                     //d_ptr->program.setUniformValue("textype", 0);
        d_ptr->textype = 0;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);    // 创建真正的数据
                                                                                                       //d_ptr->program.setUniformValue("textype", 1);
        d_ptr->textype = 1;
    }
    // 设置采样器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    DWORD t_stop = GetTickCount();
    qDebug() << "SetData:Using time : " << (t_stop - t_start) / 1000.0;
#endif

    update();
}

