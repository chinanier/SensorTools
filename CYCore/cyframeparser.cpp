#include "cyframeparser.h"
#include "cyframeparser_p.h"
#include <cycamera.h>
using namespace CYCore;
using namespace Internal;
CYFrameParserPrivate::CYFrameParserPrivate(QObject *parent)
    : QObject(parent),
    m_thr(new QThread)
{
    m_thr->start();
}

CYFrameParserPrivate::~CYFrameParserPrivate()
{

}
CYFrameParser::CYFrameParser(QObject *parent)
    : QObject(parent),
    d(new CYFrameParserPrivate(this))
{
    moveToThread(d->m_thr);
}

CYFrameParser::~CYFrameParser()
{
    
}

bool CYFrameParser::newframe(CYFRAME frame, QObject * pContext)
{
    int bret = false;
    if (QThread::currentThread()==d->m_thr)
    {
        CYFRAME sframe;
        bret = pushFullFrame(frame, sframe);
        emit sigParseCommit(frame,frame);
    }
    return bret;
}