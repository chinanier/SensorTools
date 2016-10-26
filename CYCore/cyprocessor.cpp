#include "cyprocessor.h"
#include "cyprocessor_p.h"
#include <QTimer>

using namespace CYCore;
using namespace Internal;
CYProcessorPrivate::CYProcessorPrivate(QObject *parent)
    : QObject(parent),
    m_processorThread(new QThread)
{
    m_emptybuffer.reserve(10);
    if (m_processorThread)
    {
        moveToThread(m_processorThread);
    }
    m_processorThread->start();
}

CYProcessorPrivate::~CYProcessorPrivate()
{
    if (m_processorThread)
    {
        m_processorThread->quit();
        delete m_processorThread;
        m_processorThread = 0;
    }
}
CYProcessor::CYProcessor(QObject *parent)
    : CYFrameParser(parent),
    d(new CYProcessorPrivate(this))
{

}

CYProcessor::~CYProcessor()
{

}
void CYProcessor::do_exec()
{
    pushFullFrame(CYFRAME(),CYFRAME());
}


// 内存管理接口
bool CYProcessor::AllocFrameBuffer()
{
    return true;
}
bool CYProcessor::pushEmptyFrame()
{
    return true;
}
static void fillcyframe(CYFRAME & dst,CYFRAME & src)
{
    int cpsize = dst.s_length > src.s_length ? src.s_length : dst.s_length;
    dst.s_size     = src.s_size;
    dst.s_version  = src.s_version;
    dst.s_seqno    = src.s_seqno;
    dst.s_seqidx   = src.s_seqidx;
    dst.s_id       = src.s_id;
    dst.s_bw       = src.s_bw;
    dst.s_color    = src.s_color;
    dst.s_width    = src.s_width;
    dst.s_height   = src.s_height;
    dst.s_stride   = src.s_stride;
    dst.s_pitch    = src.s_pitch;
    //dst.s_length   = src.s_length;
    //dst.s_data     = src.s_data;
    memcpy(dst.s_data, src.s_data, cpsize);
    dst.s_context  = src.s_context;
}
bool CYProcessor::pushFullFrame(CYFRAME srcframe, CYFRAME & newframe)
{
    bool bret = false;
    // 不是主线程环境
    // 放入自己的缓存buff内
    // 首先判断buff是否已经满了
    newframe = srcframe;
    if (!d->m_emptybuffer.isEmpty())
    {
        CYFRAME frame = d->m_emptybuffer.takeFirst();
        fillcyframe(frame, newframe);newframe = frame;
        d->m_fullbuffer.append(frame);
        // 在处理线程触发处理
        QTimer::singleShot(0, d, [this]() {
            // 私有线程环境
            foreach(CYFRAME frame, d->m_fullbuffer)
            {
                frame = d->m_fullbuffer.takeFirst();
                this->doProcess(frame);
                d->m_emptybuffer.append(frame);
            }
        });
        bret = true;
    }
    return bret;
}
bool CYProcessor::popupEmptyFrame()
{
    return true;
}
bool CYProcessor::popupFullFrame()
{
    return true;
}