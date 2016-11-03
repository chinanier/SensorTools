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
    qDebug() << "There is " << this << "::slotCompleteCopyFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
    // 在这里将传递到处理器的内存还给设备
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    int idx = m_frameParser.indexOf(parse);    // 这里的index为数组索引
    switch (idx)
    {
    case 0:
    {
        // 说明是第一个,直接还给自己
        emit sigCompleteFrame(frame);
    }
    break;
    case -1:
    {
        // 没有找到? 出现逻辑错误
    }
    break;
    default:
    {
        // 在这之中,还给上一个处理器
        CYFrameParser * lastParse = m_frameParser.at(idx - 1);
        if (lastParse)
        {
            QTimer::singleShot(0, lastParse, [lastParse,frame]() {
                lastParse->completeFrame(frame);
            });
        }
    }
    break;
    }
}
// 处理完成,传递
void CYCameraPrivate::slotParseCommit(CYFRAME newframe)
{
    // 这里表示处理器已经处理完一帧,将图像后续传递下去
    // 在这里将传递到处理器的内存还给设备
    qDebug() << "There is " << this << "::slotParseCommit ==> id:" << newframe.s_id << "buffer:" << newframe.s_data;
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    if (!parse)
    {
        // 程序异常
        return;
    }
    int idx = m_frameParser.indexOf(parse);    // 这里的index为数组索引
    int end_idx = m_frameParser.count() - 1;
    if (idx>=0 && idx<end_idx)
    {
        CYFrameParser * laterParse = m_frameParser.at(idx + 1);
        if (laterParse)
        {
            //改变线程上下文
            QTimer::singleShot(0, laterParse, [laterParse, newframe]() {
                bool bret = false;
                bret = laterParse->newFrame(newframe);
                if (!bret)
                {
                    // 这帧图像不能传递下去,说明图像处理压力大
                }
            });
        }
    }else if (idx == end_idx)
    {
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
    CYFrameParser * parser = m_frameParser.first();
    if (parser)
    {
        //改变线程上下文
        QTimer::singleShot(0, parser, [parser, newframe]() {
            bool bret = false;
            bret = parser->newFrame(newframe);
            if (!bret)
            {
                // 这帧图像不能传递下去,说明图像处理压力大
            }
        });
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
        d->m_frameParser.removeOne(parser);
        QObject::disconnect(parser, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // 处理器拷贝完通知调度器释放内存
        QObject::disconnect(parser, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // 处理器处理完,需要通知调度器继续传递处理
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