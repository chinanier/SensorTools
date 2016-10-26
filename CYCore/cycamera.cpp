#include "cycamera_p.h"
#include "cycamera.h"
#include "cyframeparser.h"
#include <QTimer>
using namespace CYCore;
using namespace Internal;
CYCameraPrivate::CYCameraPrivate(QObject *parent)
    : QObject(parent)
{

}

CYCameraPrivate::~CYCameraPrivate()
{

}
void CYCameraPrivate::slotParseCommit(CYFRAME frame,CYFRAME newframe)
{
    parseCommit(qobject_cast<CYFrameParser*>(sender()),frame,newframe);
    disconnect(qobject_cast<CYFrameParser*>(sender()), &CYFrameParser::sigParseCommit, this, &CYCameraPrivate::slotParseCommit);
    return;
}
void CYCameraPrivate::slotHaveNewFrame(CYFRAME newframe)
{
    CYFrameParser * parser = m_frameParser.first();
    if (parser)
    {
        //改变线程上下文
        QTimer::singleShot(0, parser, [parser,newframe,this]() {
            bool bret = false;
            QObject::connect(parser, &CYFrameParser::sigParseCommit, this, &CYCameraPrivate::slotParseCommit);
            bret = parser->newframe(newframe, this->parent());
            if (!bret)
            {
                // 这帧图像不能传递下去,说明图像处理压力大
            }
            // 这个函数返回了,就可以恢复自己的内内存
        });
    }
}
// 这个函数是处理完成的意思
void CYCameraPrivate::parseCommit(CYFrameParser*pframeParser,CYFRAME frame,CYFRAME newframe)
{
    // 到这里表示图像处理或者图像分析已经完成了
    if (!pframeParser)
    {
        return;
    }
    int cnt = m_frameParser.count();
    int i = 0;
    foreach(CYFrameParser *obj, m_frameParser) {
        i++;
        if (obj==pframeParser)
        {
            // 判断是否是最后一个,不是,取下一个进行处理
            if (cnt > i)
            {
                CYFrameParser * parser = m_frameParser.at(i); // 取下一个内容
                if (parser)
                {
                    //改变线程上下文
                    QTimer::singleShot(0, parser, [parser, frame, this]() {
                        bool bret = false;
                        QObject::connect(parser, &CYFrameParser::sigParseCommit, this, &CYCameraPrivate::slotParseCommit);
                        bret = parser->newframe(frame, this->parent());
                        if (!bret)
                        {
                            // 这帧图像不能传递下去,说明图像处理压力大
                        }
                        // 这个函数返回了,就可以恢复自己的内内存
                    });
                }
            }
            // 反之,表示整条处理链路已经跑完,直接通知分析器释放
            // 调用完下一个的处理之后，调度器需要通知处理释放内存
            obj->pushEmptyFrame();
        }
    }
    return;
}
CYCamera::CYCamera(QObject *parent)
    : QObject(parent),
    d(new CYCameraPrivate(this))
{
    qRegisterMetaType<CYFRAME>();
    connect(this, &CYCamera::sigHaveNewFrame, d, &CYCameraPrivate::slotHaveNewFrame);
}

CYCamera::~CYCamera()
{
    delete d;
    d = 0;
}
bool CYCamera::addFrameParser(CYFrameParser * newNode, CYFrameParser * before)
{
    if (!newNode)
    {
        return false;
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
    return true;
}