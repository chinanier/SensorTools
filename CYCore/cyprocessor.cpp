#include "cyprocessor.h"
#include "cyprocessor_p.h"
#include "cyframeparserfactory.h"
#include <QTimer>
#define ALLOC_BUFFER_COUNT (10)
#define ALLOC_BUFFER_WIDTH (2048)
#define ALLOC_BUFFER_HEIGHT (2048)
using namespace CYCore;
using namespace Internal;
CYProcessorPrivate::CYProcessorPrivate(QObject *parent)
    : QObject(parent)
{
}

CYProcessorPrivate::~CYProcessorPrivate()
{
    if (m_pMemory)
    {
        delete[] m_pMemory;
        m_pMemory = 0;
    }
}
bool CYProcessorPrivate::AllocFrameBuffer()
{
    if (m_pMemory)
    {
        return true;
    }
    m_pMemory = new uchar[ALLOC_BUFFER_WIDTH * ALLOC_BUFFER_HEIGHT * ALLOC_BUFFER_COUNT];
    CYFRAME frame = {0};
    frame.s_size = ALLOC_BUFFER_WIDTH * ALLOC_BUFFER_HEIGHT;
    frame.s_length = ALLOC_BUFFER_WIDTH * ALLOC_BUFFER_HEIGHT;
    for (int i=0;i<ALLOC_BUFFER_COUNT;i++)
    {
        frame.s_data = (void*)(m_pMemory+ ALLOC_BUFFER_WIDTH*ALLOC_BUFFER_HEIGHT*i);
        m_emptybuffer << frame;
    }
    return true;
}
bool CYProcessorPrivate::pushEmptyFrame(CYFRAME frame)
{
    if (m_emptybuffer.size()<ALLOC_BUFFER_COUNT)
    {
        m_emptybuffer << frame;
        return true;
    }
    return false;
}
bool CYProcessorPrivate::pushFullFrame(CYFRAME frame)
{
    if (m_fullbuffer.size() < ALLOC_BUFFER_COUNT)
    {
        m_fullbuffer << frame;
        return true;
    }
    return false;
}
bool CYProcessorPrivate::popupEmptyFrame(CYFRAME & frame)
{
    if (m_emptybuffer.size() > 0)
    {
        frame = m_emptybuffer.takeFirst();
        return true;
    }
    return false;
}
bool CYProcessorPrivate::popupFullFrame(CYFRAME & frame)
{
    if (m_fullbuffer.size() > 0)
    {
        frame = m_fullbuffer.takeFirst();
        return true;
    }
    return false;
}
CYProcessor::CYProcessor(CYFrameParserFactory * factory,QObject *parent)
    : CYFrameParser(factory,parent),
    d(new CYProcessorPrivate())
{
    
}

CYProcessor::~CYProcessor()
{
    delete d;
    d = 0;
}
// 在处理线程中
void CYProcessor::do_exec(CYFRAME & frame)
{
    doProcess(frame);
}


// 内存管理接口
bool CYProcessor::AllocFrameBuffer()
{
    return d->AllocFrameBuffer();
}
bool CYProcessor::pushEmptyFrame(CYFRAME frame)
{
    return d->pushEmptyFrame(frame);
}
bool CYProcessor::pushFullFrame(CYFRAME frame)
{
    return d->pushFullFrame(frame);
}
bool CYProcessor::popupEmptyFrame(CYFRAME & frame)
{
    return d->popupEmptyFrame(frame);
}
bool CYProcessor::popupFullFrame(CYFRAME & frame)
{
    return d->popupFullFrame(frame);
}
QWidget * CYProcessor::widget()
{
    // 窗口用于显示用?
    return nullptr;
}