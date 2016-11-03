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
// �������,��
void CYCameraPrivate::slotCompleteFrame(CYFRAME frame)
{
    qDebug() << "There is " << this << "::slotCompleteCopyFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
    // �����ｫ���ݵ����������ڴ滹���豸
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    int idx = m_frameParser.indexOf(parse);    // �����indexΪ��������
    switch (idx)
    {
    case 0:
    {
        // ˵���ǵ�һ��,ֱ�ӻ����Լ�
        emit sigCompleteFrame(frame);
    }
    break;
    case -1:
    {
        // û���ҵ�? �����߼�����
    }
    break;
    default:
    {
        // ����֮��,������һ��������
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
// �������,����
void CYCameraPrivate::slotParseCommit(CYFRAME newframe)
{
    // �����ʾ�������Ѿ�������һ֡,��ͼ�����������ȥ
    // �����ｫ���ݵ����������ڴ滹���豸
    qDebug() << "There is " << this << "::slotParseCommit ==> id:" << newframe.s_id << "buffer:" << newframe.s_data;
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    if (!parse)
    {
        // �����쳣
        return;
    }
    int idx = m_frameParser.indexOf(parse);    // �����indexΪ��������
    int end_idx = m_frameParser.count() - 1;
    if (idx>=0 && idx<end_idx)
    {
        CYFrameParser * laterParse = m_frameParser.at(idx + 1);
        if (laterParse)
        {
            //�ı��߳�������
            QTimer::singleShot(0, laterParse, [laterParse, newframe]() {
                bool bret = false;
                bret = laterParse->newFrame(newframe);
                if (!bret)
                {
                    // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����
                }
            });
        }
    }else if (idx == end_idx)
    {
        // �������һ��,ֱ�ӽ��ڴ滹�����һ��
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
        //�ı��߳�������
        QTimer::singleShot(0, parser, [parser, newframe]() {
            bool bret = false;
            bret = parser->newFrame(newframe);
            if (!bret)
            {
                // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����
            }
        });
    }
}
// ��������Ǵ�����ɵ���˼
CYCamera::CYCamera(QObject *parent)
    : QObject(parent),
    d(new CYCameraPrivate(this))
{
    qRegisterMetaType<CYFRAME>();
    connect(this, &CYCamera::sigHaveNewFrame, d, &CYCameraPrivate::slotHaveNewFrame);    // ���豸����ͼ���ź�
    connect(d, &CYCameraPrivate::sigCompleteFrame, this, &CYCamera::sigCompleteFrame);   // �󶨴������һ֡,����֪ͨ�豸�ͷ��ڴ�
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
    QObject::connect(newNode, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // ������������֪ͨ�������ͷ��ڴ�
    QObject::connect(newNode, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // ������������,��Ҫ֪ͨ�������������ݴ���
    return true;
}
bool CYCamera::delFrameParser(CYFrameParser * parser)
{
    if (d->m_frameParser.contains(parser))
    {
        d->m_frameParser.removeOne(parser);
        QObject::disconnect(parser, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // ������������֪ͨ�������ͷ��ڴ�
        QObject::disconnect(parser, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // ������������,��Ҫ֪ͨ�������������ݴ���
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