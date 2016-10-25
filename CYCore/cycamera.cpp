#include "cycamera_p.h"
#include "cycamera.h"
#include "cyframeparser.h"
using namespace CYCore;
using namespace Internal;
CYCameraPrivate::CYCameraPrivate(QObject *parent)
    : QObject(parent)
{

}

CYCameraPrivate::~CYCameraPrivate()
{

}
void CYCameraPrivate::slotParseCommit()
{
    parseCommit(qobject_cast<CYFrameParser*>(sender()));
    return;
}
void CYCameraPrivate::slotHaveNewFrame()
{
    CYFrameParser * parser = m_frameParser.first();
    if (parser)
    {
        parser->pushFullFrame();
        parser->do_exec();
    }
}
void CYCameraPrivate::parseCommit(CYFrameParser*pframeParser)
{
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
                CYFrameParser * parser = m_frameParser.at(i);
                if (parser)
                {
                    parser->pushFullFrame();
                    parser->do_exec();
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
    connect(this, SIGNAL(sigHaveNewFrame()), d, SLOT(slotHaveNewFrame()));
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