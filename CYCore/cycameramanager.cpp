#include "cycameramanager.h"
#include "cycamera.h"
#include "cycameramanager_p.h"
using namespace CYCore;
using namespace Internal;

// ---------------- EditorManager

static CYCameraManager *m_instance = 0;
static CYCameraManagerPrivate *d;


CYCameraManagerPrivate::CYCameraManagerPrivate(QObject *parent)
    : QObject(parent)
{

}

CYCameraManagerPrivate::~CYCameraManagerPrivate()
{

}

CYCameraManager *CYCameraManager::instance()
{
    return m_instance;
}
bool CYCameraManager::SerachCamera(Id id)
{
    // δָ��Idʱ��Ҫ�������еĹ����������������ӿ�

    // 1����ϵͳ����л�ȡCYCameraFactory��

    // 2��ͨ�������ഴ��CYCameraʵ��

    // 3��ͨ���������������ӿ�

    // 4���ͷ������

    return false;
}

bool CYCameraManager::connectCamera(Id id, int chl)
{
    // δָ��Idʱ�����õ�һ�����������������ӽӿ�

    // 1����ϵͳ����л�ȡCYCameraFactory��

    // 2��ͨ�������ഴ��CYCameraʵ��

    // 3��ͨ��������������

    // 4�������ʵ�����浽�����з����ȡ

    return false;
}
bool CYCameraManager::disconnectCamera(Id id, int chl)
{
    // δָ��Idʱ��ֱ�ӷ��أ���������

    // 1���������б��л�ȡ���е����

    // 2����ȡָ��ͨ�����

    // 3������ҵ�������öϿ��ӿ�

    // 4�������ʵ�����浽�����з����ȡ

    // 5�����Ͽ����ӵ�����Ƴ��б�(Ҳ�����ö�ʱ�������)

    return false;
}

bool CYCameraManager::startCapture()
{
    return false;
}
bool CYCameraManager::stopCapture()
{
    return false;
}

bool CYCameraManager::sendCommand()
{
    return false;
}
bool CYCameraManager::readCommand()
{
    return false;
}

CYCamera * CYCameraManager::createCamera(Id id)
{
    return 0;
}
CYCamera * CYCameraManager::currentCamera()
{
    return 0;
}
void CYCameraManager::activateCamera(CYCamera *pCamera)
{
    return ;
}
CYCameraManager::CYCameraManager(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
    d = new CYCameraManagerPrivate(this);
}

CYCameraManager::~CYCameraManager()
{
    delete d;
    d = 0;
}
