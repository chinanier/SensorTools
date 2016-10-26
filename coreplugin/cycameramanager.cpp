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
    // δָ��Idʱ��Ҫ�������еĹ����������������ӿ�
    if (!factoryid.isValid())
    {
        // ������Ч�ĵ������е�
        return 0;
    }
    // ����ʱ,���ʵ����û�г�ʼ��
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
// cameraid��factoryid�������,��factory�����õ���camera����".index",�硰ACC.1��
bool CYCameraManager::connectCamera(Id cameraid, int chl)
{
    // δָ��Idʱ�����õ�һ�����������������ӽӿ�

    // 1����ϵͳ����л�ȡCYCameraFactory��

    // 2��ͨ�������ഴ��CYCameraʵ��

    // 3��ͨ��������������

    // 4�������ʵ�����浽�����з����ȡ

    CYCamera * pCamera = d->getCameraForCameraID(cameraid);
    return pCamera ? pCamera->connectCamera(chl) : false;
}
bool CYCameraManager::disconnectCamera(Id cameraid, int chl)
{
    // δָ��Idʱ��ֱ�ӷ��أ���������

    // 1���������б��л�ȡ���е����

    // 2����ȡָ��ͨ�����

    // 3������ҵ�������öϿ��ӿ�

    // 4�������ʵ�����浽�����з����ȡ

    // 5�����Ͽ����ӵ�����Ƴ��б�(Ҳ�����ö�ʱ�������)
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

// ��ʼ������������ͨ���ú�����ʼ��ע��������(ͼ�ദ��ͼ��ͳ��,����׼���ŵ�ͼ�����������й���)
void CYCameraManager::init()
{
    // ��ȡϵͳ���ڵ��������������ϵͳͨ������Ľӿ���������ӿ��б�
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
            // ����model
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