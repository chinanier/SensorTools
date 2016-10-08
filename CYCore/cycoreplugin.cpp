/*#include "cycore.h"*/
#include "mainwindow.h"
#include "cycoreplugin.h"
#include "actionmanager.h"
using namespace CYCore;
using namespace CYCore::Internal;

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
    new ActionManager(this);
    m_mainWindow = new MainWindow;
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
    if (m_mainWindow)
    {
        m_mainWindow->show();
    }
}
