#include "cyprocessor.h"
#include "cyprocessor_p.h"
using namespace CYCore;
using namespace Internal;
CYProcessorPrivate::CYProcessorPrivate(QObject *parent)
    : QObject(parent),
    m_processorThread(new QThread)
{
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
    : CYFrameParser(parent)
{

}

CYProcessor::~CYProcessor()
{

}
void CYProcessor::do_exec()
{
    pushFullFrame();
}


// 内存管理接口
void CYProcessor::AllocFrameBuffer()
{

}
void CYProcessor::pushEmptyFrame()
{
    return;
}

void CYProcessor::pushFullFrame()
{
    return;
}
void CYProcessor::popupEmptyFrame()
{
    return;
}
void CYProcessor::popupFullFrame()
{
    return;
}