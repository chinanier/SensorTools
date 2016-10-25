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
            // �ж��Ƿ������һ��,����,ȡ��һ�����д���
            if (cnt > i)
            {
                CYFrameParser * parser = m_frameParser.at(i);
                if (parser)
                {
                    parser->pushFullFrame();
                    parser->do_exec();
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