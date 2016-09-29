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
    // 未指定Id时，要调用所有的工厂类的相机的搜索接口

    // 1、从系统插件中获取CYCameraFactory类

    // 2、通过工厂类创建CYCamera实例

    // 3、通过相机类调用搜索接口

    // 4、释放相机类

    return false;
}

bool CYCameraManager::connectCamera(Id id, int chl)
{
    // 未指定Id时，调用第一个工厂类的相机的链接接口

    // 1、从系统插件中获取CYCameraFactory类

    // 2、通过工厂类创建CYCamera实例

    // 3、通过相机类调用链接

    // 4、将相机实例保存到链表中方便获取

    return false;
}
bool CYCameraManager::disconnectCamera(Id id, int chl)
{
    // 未指定Id时，直接返回，不作处理

    // 1、从链接列表中获取所有的相机

    // 2、获取指定通道相机

    // 3、如果找到，则调用断开接口

    // 4、将相机实例保存到链表中方便获取

    // 5、将断开链接的相机移除列表(也可以用定时清理机制)

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
