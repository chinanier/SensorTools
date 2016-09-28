#include "plugin1.h"
#include "qglobal.h"
#include <QtCore/QtPlugin>
#include "plugin1plugin.h"


Plugin1Plugin::Plugin1Plugin()
{
   initializeCalled = false;
}
Plugin1Plugin::~Plugin1Plugin()
{
    initializeCalled = initializeCalled;
}
bool Plugin1Plugin::initialize(const QStringList & /*arguments*/, QString *errorString)
{
    initializeCalled = true;
    QObject *obj = new QObject(this);
    obj->setObjectName("MyPlugin1");
    addAutoReleasedObject(obj);
    return true;
}

void Plugin1Plugin::extensionsInitialized()
{
    if (!initializeCalled)
        return;
    // don't do this at home, it's just done here for the test
    QObject *obj = new QObject(this);
    obj->setObjectName("MyPlugin1_running");
    addAutoReleasedObject(obj);
}

