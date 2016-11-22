#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>
#include "cycore_global.h"

class CYGLImageAreaPrivate;
class CYCORE_EXPORT CYGLImageArea : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    CYGLImageArea(QWidget *parent = Q_NULLPTR);
    ~CYGLImageArea();

public:
    bool ZoomPlus(const bool bOn_i);                  // 放大
    bool ZoomMin(const bool bOn_i);                   // 缩小
    bool ZoomOneToOne();                              // 1::1
    bool MakeToCenter();                              // 中心显示

    //void SetData(void * data);                                   // 更新图像数据
    void setData(int width, int height, uint sourceFormat, uint sourceType,const void *data);
protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
signals:
    void initialed();
private:
    friend class CYGLImageAreaPrivate;
    CYGLImageAreaPrivate * d_ptr;
};
