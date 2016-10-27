#include "cyframeparser.h"
#include "cyframeparser_p.h"
#include <cycamera.h>

#include <QTimer>
#include <QDebug>
using namespace CYCore;
using namespace Internal;
static void fillcyframe(CYFRAME & dst, CYFRAME & src)
{
    uint cpsize = dst.s_length > src.s_length ? src.s_length : dst.s_length;
    dst.s_size = src.s_size;
    dst.s_version = src.s_version;
    dst.s_seqno = src.s_seqno;
    dst.s_seqidx = src.s_seqidx;
    dst.s_id = src.s_id;
    dst.s_bw = src.s_bw;
    dst.s_color = src.s_color;
    dst.s_width = src.s_width;
    dst.s_height = src.s_height;
    dst.s_stride = src.s_stride;
    dst.s_pitch = src.s_pitch;
    //dst.s_length   = src.s_length;
    //dst.s_data     = src.s_data;
    memcpy(dst.s_data, src.s_data, cpsize);
    dst.s_context = src.s_context;
}
CYFrameParserPrivate::CYFrameParserPrivate(QObject *parent)
    : QObject(parent),
    m_thr(new QThread),
    m_thrParser(new QThread)
{
    m_thr->start();
    m_thrParser->start();
    moveToThread(m_thrParser);
}
CYFrameParserPrivate::~CYFrameParserPrivate()
{
    m_thr->quit();
    m_thrParser->quit();
    m_thr->wait(-1);
    m_thrParser->wait(-1);
    delete m_thr;
    delete m_thrParser;
}
void CYFrameParserPrivate::do_exec()
{
    CYFRAME frame;
    if (m_parent)
    {
        while (m_parent->popupFullFrame(frame))
        {
            m_parent->do_exec(frame);
            emit sigParseCommit(frame);    // 通知父类-》通知调度器--处理完成
        }
    }
}
// 
CYFrameParser::CYFrameParser(QObject *parent)
    : QObject(parent),
    d(new CYFrameParserPrivate())
{
    d->m_parent = this;
    moveToThread(d->m_thr);
    connect(d, &CYFrameParserPrivate::sigParseCommit,this,&CYFrameParser::sigParseCommit);
}

CYFrameParser::~CYFrameParser()
{
    delete d;
    d = 0;
}
bool CYFrameParser::newFrame(CYFRAME frame)
{
    qDebug() << "There is "<< this <<":newFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
    int bret = false;
    if (QThread::currentThread()==d->m_thr)
    {
        CYFRAME sframe;
        if (popupEmptyFrame(sframe))
        {
            fillcyframe(sframe,frame);
            bret = pushFullFrame(sframe);
            QTimer::singleShot(0, d, [this]() {
                d->do_exec();
            });
        }
        emit sigFrameCopyCommit(frame);          // 通知调度器-处理器已经将一帧拷贝完
    }
    return bret;
}
void CYFrameParser::completeFrame(CYFRAME frame)
{
    pushEmptyFrame(frame);
    return;
}