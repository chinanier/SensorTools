#include "cycamera_p.h"
#include "cycamera.h"
#include "cyframeparser.h"

#include "cyframeparserfactory.h"

#include <QTimer>
#include <QDebug>
using namespace CYCore;
using namespace Internal;
CYCameraPrivate::CYCameraPrivate(QObject *parent)
    : QObject(parent)
{

}

CYCameraPrivate::~CYCameraPrivate()
{

}
// 拷贝完成,回
void CYCameraPrivate::slotCompleteFrame(CYFRAME frame)
{
    //qDebug() << "There is " << this << "::slotCompleteCopyFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
    // 在这里将传递到处理器的内存还给设备
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    int idx = m_frameParser.indexOf(parse);    // 这里的index为数组索引
    CYFrameParser*product = qobject_cast<CYFrameParser*>((QObject*)frame.s_context);
    CYCameraPrivate*product_cam = qobject_cast<CYCameraPrivate*>((QObject*)frame.s_context);
    if (product_cam)
    {
        // 说明是第一个,直接还给自己
        emit sigCompleteFrame(frame);
    }
    else
    {
        QTimer::singleShot(0, product, [product, frame]() {
            product->completeFrame(frame);
        });
    }
}
// 处理完成,传递
void CYCameraPrivate::slotParseCommit(CYFRAME newframe)
{
    // 这里表示处理器已经处理完一帧,将图像后续传递下去
    // 在这里将传递到处理器的内存还给设备
    //qDebug() << "There is " << this << "::slotParseCommit ==> id:" << newframe.s_id << "buffer:" << newframe.s_data;
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    if (!parse)
    {
        // 程序异常
        return;
    }
    int idx = m_frameParser.indexOf(parse);    // 这里的index为数组索引
    int end_idx = m_frameParser.count() - 1;
    CYFrameParser * laterParse = 0;
    // 从自己开始之后找，找到下一级使能的,否则将buff还给自己
    for (int i=idx+1;i<end_idx+1;i++)
    {
        CYFrameParser * p = m_frameParser.at(i);
        if (p&&p->isEnabled())
        {
            laterParse = p;
            break;
        }
    }
    if (laterParse)
    {
        newframe.s_context = parse; // 指定上一级的生产者
        //改变线程上下文
        QTimer::singleShot(0, laterParse, [laterParse, newframe]() {
            bool bret = false;
            bret = laterParse->newFrame(newframe);
            if (!bret)
            {
                // 这帧图像不能传递下去,说明图像处理压力大,已回收
            }
        });
    }
    else
    {
        // 没有找到下级处理,直接将内存回收
        // 处理到最后一个,直接将内存还给最后一个
        QTimer::singleShot(0, parse, [parse, newframe]() {
            parse->completeFrame(newframe);
        });
    }
    return;
}
void CYCameraPrivate::slotHaveNewFrame(CYFRAME newframe)
{
    qDebug() << "There is CYCameraPrivate::slotHaveNewFrame==> id:" << newframe.s_id << "buffer:" << newframe.s_data;
    CYFrameParser * parser = 0;//m_frameParser.first();
    for (int i=0;i<m_frameParser.size();i++)
    {
        CYFrameParser * p = m_frameParser.at(i);
        if (p&&p->isEnabled())
        {
            parser = p;
            break;
        }
    }
    newframe.s_context = this;
    if (parser)
    {
        //改变线程上下文
        QTimer::singleShot(0, parser, [parser, newframe]() {
            bool bret = false;
            if (parser->isEnabled())
            {
                bret = parser->newFrame(newframe);
                if (!bret)
                {
                    // 这帧图像不能传递下去,说明图像处理压力大
                }
            }
        });
    }
    else
    {
        // 没有找到处理器，直接返回
        emit sigCompleteFrame(newframe);
    }
}
// 这个函数是处理完成的意思
CYCamera::CYCamera(QObject *parent)
    : QObject(parent),
    d(new CYCameraPrivate(this))
{
    qRegisterMetaType<CYFRAME>();
    connect(this, &CYCamera::sigHaveNewFrame, d, &CYCameraPrivate::slotHaveNewFrame);    // 绑定设备的新图像信号
    connect(d, &CYCameraPrivate::sigCompleteFrame, this, &CYCamera::sigCompleteFrame);   // 绑定处理完成一帧,用于通知设备释放内存
}

CYCamera::~CYCamera()
{
    delete d;
    d = 0;
}
CYCameraFactory * CYCamera::factory()
{
    return d->m_pfactory;
}
void CYCamera::setFactory(CYCameraFactory*factory)
{
    d->m_pfactory = factory;
}
bool CYCamera::connectCameraHelper(int chl)
{
    bool ret = false;
    if (ret = connectCamera(chl))
    {
        emit sigConnectChange(chl);
    }
    return ret;
}
bool CYCamera::disconnectCameraHelper(int chl)
{
    bool ret = false;
    if (ret = disconnectCamera(chl))
    {
        emit sigConnectChange(chl);
    }
    return ret;
}
bool CYCamera::startCaptureHelper(int chl)
{
    bool ret = false;
    if (ret = startCapture(chl))
    {
        emit sigCaptureChange(chl);
    }
    return ret;
}
bool CYCamera::stopCaptureHelper(int chl)
{
    bool ret = false;
    if (ret = stopCapture(chl))
    {
        emit sigCaptureChange(chl);
    }
    return ret;
}
bool CYCamera::addFrameParser(CYFrameParser * newNode, CYFrameParser * before)
{
    if (!newNode)
    {
        return false;
    }
    if (d->m_frameParser.contains(newNode))
    {
        return true;
    }
    if (d->m_frameParser.contains(before))
    {
        int index = d->m_frameParser.indexOf(before);
        d->m_frameParser.insert(index, newNode);
    }
    else
    {
        d->m_frameParser.append(newNode);
    }
    QObject::connect(newNode, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // 处理器拷贝完通知调度器释放内存
    QObject::connect(newNode, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // 处理器处理完,需要通知调度器继续传递处理
    return true;
}
bool CYCamera::delFrameParser(CYFrameParser * parser)
{
    if (d->m_frameParser.contains(parser))
    {
        auto func = [this, parser]() {
            // 完成内存回收,从处理链路中移除
            d->m_frameParser.removeOne(parser);
            QObject::disconnect(parser, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // 处理器拷贝完通知调度器释放内存
            QObject::disconnect(parser, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // 处理器处理完,需要通知调度器继续传递处理
            delete parser;
        };
        parser->setEnabled(false);    // 关闭处理,等待所有的内存回归
        if (parser->isFull(BUFFER_EMPTY))
        {
            func();
        }
        else
        {
            connect(parser, &CYFrameParser::sigBufferRecoveryFinish, this, func);
        }
        return true;
    }
    return false;
}

QList<CYFrameParser*> CYCamera::frameParser(Id factoryid)
{
    if (factoryid.isValid())
    {
        QList<CYFrameParser*> l;
        foreach(CYFrameParser *p,d->m_frameParser)
        {
            if (p->factory())
            {
                if (p->factory()->id() == factoryid)
                {
                    l.append(p);
                }
            }
        }
        return l;
    }
    else
    {
        return d->m_frameParser;
    }
}