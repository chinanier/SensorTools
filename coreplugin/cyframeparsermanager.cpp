#include "cyframeparsermanager_p.h"
#include "cyframeparsermanager.h"

#include <CYCore/cyframeparserfactory.h>

#include <ExtensionSystem/pluginmanager.h>
using namespace CYCore;
using namespace CYCore::Internal;

static CYFrameParserManager *m_instance = 0;
static CYFrameParserManagerPrivate *d;

CYFrameParserManagerPrivate::CYFrameParserManagerPrivate(QObject *parent)
    : QObject(parent)
{

}

CYFrameParserManagerPrivate::~CYFrameParserManagerPrivate()
{

}

CYFrameParserManager::CYFrameParserManager(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
    d = new CYFrameParserManagerPrivate(this);
}

CYFrameParserManager::~CYFrameParserManager()
{

}

CYFrameParserManager *CYFrameParserManager::instance()
{
    return m_instance;
}
void CYFrameParserManager::init()
{
    // ��ȡϵͳ���ڵ��������������ϵͳͨ������Ľӿ���������ӿ��б�
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::objectAdded,
        m_instance, &CYFrameParserManager::objectAdded);
    QObject::connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::aboutToRemoveObject,
        m_instance, &CYFrameParserManager::aboutToRemoveObject);
    return;
    return;
}
void CYFrameParserManager::objectAdded(QObject *obj)
{
    CYFrameParserFactory * factory = qobject_cast<CYFrameParserFactory *>(obj);
    if (factory)
    {
        //CYProcessFactoryList & fl = d->m_factorysOfid[factory->id()];
        if (!d->m_factorysOfid.contains(factory->id()))
        {
            d->m_factorysOfid[factory->id()] = factory;
        }
        else
        {
            // ȡ�����ظ���id��???
        }
    }
    return;
}
void CYFrameParserManager::aboutToRemoveObject(QObject *obj)
{
    CYFrameParserFactory * factory = qobject_cast<CYFrameParserFactory *>(obj);
    if (factory)
    {
        if (d->m_factorysOfid.contains(factory->id()))
        {
            d->m_factorysOfid.remove(factory->id());
        }
    }
}
CYFrameParserManager::CYFrameParserFactoryList CYFrameParserManager::getProcessorFactorys(Utils::Id id)
{
    CYFrameParserManager::CYFrameParserFactoryList factorys;
    if (id.isValid())
    {
        CYFrameParserFactory * p = d->m_factorysOfid[id];
        factorys.append(p);
    }
    else
    {
        foreach(CYFrameParserFactory * p , d->m_factorysOfid)
        {
            factorys.append(p);
        }
    }
    return factorys;
}
void CYFrameParserManager::destroy()
{
    
}