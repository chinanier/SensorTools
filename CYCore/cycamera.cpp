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
    //qDebug() << "There is " << this << "::slotCompleteCopyFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
    // �����ｫ���ݵ����������ڴ滹���豸
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    int idx = m_frameParser.indexOf(parse);    // �����indexΪ��������
    CYFrameParser*product = qobject_cast<CYFrameParser*>((QObject*)frame.s_context);
    CYCameraPrivate*product_cam = qobject_cast<CYCameraPrivate*>((QObject*)frame.s_context);
    if (product_cam)
    {
        // ˵���ǵ�һ��,ֱ�ӻ����Լ�
        emit sigCompleteFrame(frame);
    }
    else
    {
        QTimer::singleShot(0, product, [product, frame]() {
            product->completeFrame(frame);
        });
    }
}
// �������,����
void CYCameraPrivate::slotParseCommit(CYFRAME newframe)
{
    // �����ʾ�������Ѿ�������һ֡,��ͼ�����������ȥ
    // �����ｫ���ݵ����������ڴ滹���豸
    //qDebug() << "There is " << this << "::slotParseCommit ==> id:" << newframe.s_id << "buffer:" << newframe.s_data;
    CYFrameParser* parse = qobject_cast<CYFrameParser*>(sender());
    if (!parse)
    {
        // �����쳣
        return;
    }
    int idx = m_frameParser.indexOf(parse);    // �����indexΪ��������
    int end_idx = m_frameParser.count() - 1;
    CYFrameParser * laterParse = 0;
    // ���Լ���ʼ֮���ң��ҵ���һ��ʹ�ܵ�,����buff�����Լ�
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
        newframe.s_context = parse; // ָ����һ����������
        //�ı��߳�������
        QTimer::singleShot(0, laterParse, [laterParse, newframe]() {
            bool bret = false;
            bret = laterParse->newFrame(newframe);
            if (!bret)
            {
                // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����,�ѻ���
            }
        });
    }
    else
    {
        // û���ҵ��¼�����,ֱ�ӽ��ڴ����
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
        //�ı��߳�������
        QTimer::singleShot(0, parser, [parser, newframe]() {
            bool bret = false;
            if (parser->isEnabled())
            {
                bret = parser->newFrame(newframe);
                if (!bret)
                {
                    // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����
                }
            }
        });
    }
    else
    {
        // û���ҵ���������ֱ�ӷ���
        emit sigCompleteFrame(newframe);
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
    QObject::connect(newNode, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // ������������֪ͨ�������ͷ��ڴ�
    QObject::connect(newNode, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // ������������,��Ҫ֪ͨ�������������ݴ���
    return true;
}
bool CYCamera::delFrameParser(CYFrameParser * parser)
{
    if (d->m_frameParser.contains(parser))
    {
        auto func = [this, parser]() {
            // ����ڴ����,�Ӵ�����·���Ƴ�
            d->m_frameParser.removeOne(parser);
            QObject::disconnect(parser, &CYFrameParser::sigFrameCopyCommit, d, &CYCameraPrivate::slotCompleteFrame); // ������������֪ͨ�������ͷ��ڴ�
            QObject::disconnect(parser, &CYFrameParser::sigParseCommit, d, &CYCameraPrivate::slotParseCommit);       // ������������,��Ҫ֪ͨ�������������ݴ���
            delete parser;
        };
        parser->setEnabled(false);    // �رմ���,�ȴ����е��ڴ�ع�
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