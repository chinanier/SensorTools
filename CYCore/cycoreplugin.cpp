#include "cycore.h"

#include <QtCore/QtPlugin>
#include "cycoreplugin.h"


CYCorePlugin::CYCorePlugin()
{
    initializeCalled = false;
}
CYCorePlugin::~CYCorePlugin()
{
    initializeCalled = initializeCalled;
}
bool CYCorePlugin::initialize(const QStringList & /*arguments*/, QString *errorString)
{
    initializeCalled = true;
    QObject *obj = new QObject(this);
    obj->setObjectName("MyPlugin1");
    addAutoReleasedObject(obj);
    return true;
}

void CYCorePlugin::extensionsInitialized()
{
    if (!initializeCalled)
        return;
    // don't do this at home, it's just done here for the test
    QObject *obj = new QObject(this);
    obj->setObjectName("MyPlugin1_running");
    addAutoReleasedObject(obj);
}
