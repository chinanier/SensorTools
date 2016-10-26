#include <cycore/cycamerafactory.h>
#include <cycore/cycamera.h>
#include <CYCore/cyframeparser.h>

#include "ExtensionSystem/pluginmanager.h"

#include "cycameramanager.h"
#include "cycameramanager_p.h"
#include "cycameracategorymode.h"


using namespace Utils;
using namespace CYCore;
using namespace Internal;


static CYCameraManager *m_instance = 0;
static CYCameraManagerPrivate *d;


CYCameraManagerPrivate::CYCameraManagerPrivate(QObject *parent)
    : QObject(parent)
{

}

CYCameraManagerPrivate::~CYCameraManagerPrivate()
{

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
CYCamera * CYCameraManagerPrivate::getCameraForCameraID(Utils::Id cameraid)
{
    QVariant varid = cameraid.toSetting();
    QString strid = varid.toString();
    QStringList strlist = strid.split('.');
    CYCamera * pcamera = 0;
    if (strlist.size() > 1)
    {
        QString camid = strlist[strlist.size() - 1];
        int cameraidsize = camid.size();
        Id factoryid = Id::fromSetting(strid.left(strid.size() - cameraidsize - 1));
        int cameraindex = camid.toInt();
        QList<CYCamera*>cameraList = m_idOfCameras[factoryid];
        if (cameraList.size() > cameraindex)
        {
            pcamera = cameraList.at(cameraindex);
        }
    }
    return pcamera;
}
Utils::Id CYCameraManagerPrivate::getFactoryIdForCameraID(Utils::Id cameraid)
{
    QVariant varid = cameraid.toSetting();
    QString strid = varid.toString();
    QStringList strlist = strid.split('.');
    Utils::Id factoryid = 0;
    if (strlist.size() > 1)
    {
        QString camid = strlist[strlist.size() - 1];
        int cameraidsize = camid.size();
        factoryid = Id::fromSetting(strid.left(strid.size() - cameraidsize - 1));
    }
    return factoryid;
}
void CYCameraManager::destroy()
{
    QList<CYCameraCategoryMode*> modles = ExtensionSystem::PluginManager::getObjects<CYCameraCategoryMode>();
    foreach(CYCameraCategoryMode*modle, modles)
    {
        ExtensionSystem::PluginManager::removeObject(modle);
        delete modle;
    }
}
CYCameraManager *CYCameraManager::instance()
{
    return m_instance;
}
int CYCameraManager::SerachCamera(Id factoryid, uint timeout)
{
    // 未指定Id时，要调用所有的工厂类的相机的搜索接口
    if (!factoryid.isValid())
    {
        // 不是有效的调用所有的
        return 0;
    }
    // 搜索时,相机实例并没有初始化
    CYCameraFactory * factory = d->m_idOfFactorys[factoryid];
    int cameracount = 0;
    if (factory)
    {
        cameracount = factory->SerarchCamera();
        for (int i=0;i<cameracount;i++)
        {
            d->m_idOfCameras[factoryid].append(factory->createCamera());
        }
    }
    return cameracount;
}
// cameraid由factoryid进行组合,由factory搜索得到的camera加上".index",如“ACC.1”
bool CYCameraManager::connectCamera(Id cameraid, int chl)
{
    // 未指定Id时，调用第一个工厂类的相机的链接接口

    // 1、从系统插件中获取CYCameraFactory类

    // 2、通过工厂类创建CYCamera实例

    // 3、通过相机类调用链接

    // 4、将相机实例保存到链表中方便获取

    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->connectCamera(chl) : false;
}
bool CYCameraManager::disconnectCamera(Id cameraid, int chl)
{
    // 未指定Id时，直接返回，不作处理

    // 1、从链接列表中获取所有的相机

    // 2、获取指定通道相机

    // 3、如果找到，则调用断开接口

    // 4、将相机实例保存到链表中方便获取

    // 5、将断开链接的相机移除列表(也可以用定时清理机制)
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->disconnectCamera(chl) : false;
}
bool CYCameraManager::isConnect(Utils::Id cameraid, int chl)
{
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->isConnect(chl) : false;
}
bool CYCameraManager::startCapture(Utils::Id cameraid, int chl)
{
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->startCapture(chl) : false;
}
bool CYCameraManager::stopCapture(Utils::Id cameraid, int chl)
{
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->stopCapture(chl) : false;
}
bool CYCameraManager::isCapture(Utils::Id cameraid, int chl)
{
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->isCapture(chl) : false;
}
bool CYCameraManager::sendCommand()
{
    return false;
}
bool CYCameraManager::readCommand()
{
    return false;
}

CYCamera * CYCameraManager::createCamera(Id cameraid)
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

bool CYCameraManager::appendFrameParser(Utils::Id cameraid, CYFrameParser * parser)
{
    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera? pCamera->addFrameParser(parser) : false;
}

// 初始化操作，可能通过该函数初始化注册的相机，(图相处理，图像统计,后期准备放到图像处理管理类进行管理)
void CYCameraManager::init()
{
    // 获取系统存在的相机工厂类插件，系统通过该类的接口生成相机接口列表
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::objectAdded,
        m_instance, &CYCameraManager::objectAdded);
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::aboutToRemoveObject,
        m_instance, &CYCameraManager::aboutToRemoveObject);
    return ;
}
void CYCameraManager::objectAdded(QObject *obj)
{
    CYCameraFactory * factory = qobject_cast<CYCameraFactory *>(obj);
    if (factory)
    {
        CYCameraFactoryList & fl = d->m_categoryOfFactory[factory->category()];
        if (fl.isEmpty())
        {
            // 创建model
            CYCameraCategoryMode * modeView = new CYCameraCategoryMode(factory->category().toStdString().c_str());
            modeView->setDisplayName(factory->category());
            modeView->setIcon(factory->icon());
            modeView->setPriority(factory->priority());
            ExtensionSystem::PluginManager::addObject(modeView);
        }
        fl << factory;
        d->m_idOfFactorys[factory->id()] = factory;
    }
    return;
}
void CYCameraManager::aboutToRemoveObject(QObject *obj)
{
    CYCameraFactory * factory = qobject_cast<CYCameraFactory *>(obj);
    if (factory)
    {
        CYCameraFactoryList & fl = d->m_categoryOfFactory[factory->category()];
        if (fl.contains(factory))
        {
            fl.removeOne(factory);
        }
        d->m_idOfFactorys.remove(factory->id());
    }
}
CYCameraManager::CYCameraFactoryList CYCameraManager::getCameraFactorys(Id category)
{
    if (!category.isValid())
    {
        return ExtensionSystem::PluginManager::getObjects<CYCameraFactory>();
    }
    if (d->m_categoryOfFactory.contains(category.toString()))
    {
        return d->m_categoryOfFactory[category.toString()];
    }
    return CYCameraManager::CYCameraFactoryList();
}