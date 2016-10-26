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
        //�ı��߳�������
        QTimer::singleShot(0, parser, [parser,newframe,this]() {
            bool bret = false;
            QObject::connect(parser, &CYFrameParser::sigParseCommit, this, &CYCameraPrivate::slotParseCommit);
            bret = parser->newframe(newframe, this->parent());
            if (!bret)
            {
                // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����
            }
            // �������������,�Ϳ��Իָ��Լ������ڴ�
        });
    }
}
// ��������Ǵ�����ɵ���˼
void CYCameraPrivate::parseCommit(CYFrameParser*pframeParser,CYFRAME frame,CYFRAME newframe)
{
    // �������ʾͼ�������ͼ������Ѿ������
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
            // �ж��Ƿ������һ��,����,ȡ��һ�����д���
            if (cnt > i)
            {
                CYFrameParser * parser = m_frameParser.at(i); // ȡ��һ������
                if (parser)
                {
                    //�ı��߳�������
                    QTimer::singleShot(0, parser, [parser, frame, this]() {
                        bool bret = false;
                        QObject::connect(parser, &CYFrameParser::sigParseCommit, this, &CYCameraPrivate::slotParseCommit);
                        bret = parser->newframe(frame, this->parent());
                        if (!bret)
                        {
                            // ��֡ͼ���ܴ�����ȥ,˵��ͼ����ѹ����
                        }
                        // �������������,�Ϳ��Իָ��Լ������ڴ�
                    });
                }
            }
            // ��֮,��ʾ����������·�Ѿ�����,ֱ��֪ͨ�������ͷ�
            // ��������һ���Ĵ���֮�󣬵�������Ҫ֪ͨ�����ͷ��ڴ�
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