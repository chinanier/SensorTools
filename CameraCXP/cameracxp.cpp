#include "cameracxp.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/findplaceholder.h>
#include <coreplugin/icore.h>
#include <coreplugin/navigationwidget.h>
#include <coreplugin/outputpane.h>
#include <coreplugin/rightpane.h>
#include <coreplugin/imode.h>

#include <coreplugin/cycameramanager.h>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>

#include <CYCore/cyframeparserfactory.h>
#include <CYCore/cyanalyzer.h>
#include <CYCore/cyframeparser.h>
#include <CYCore/cyprocessor.h>

#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QStackedWidget>
#include <QToolButton>
#include <QMessageBox>

using namespace Core;
using namespace CYCore;

namespace CameraCXP {
namespace Internal {

class TestFrameParser : public CYCore::CYProcessor
{
public:
    virtual void doProcess()
    {
        int i = 0;
        i = i;
    }
};

class TestProcessor : public CYCore::CYFrameParserFactory {
public:
    TestProcessor()
    {
        setId("TESTPROCESSOR");
        setDisplayName("TestProcessor");
        setType(CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR);
    }
    ~TestProcessor()
    {

    }
    CYFrameParser *createFrameParser() {
        return new TestFrameParser;
    }
};


class CoaxPressCamera : public CYCore::CYCamera
{
public:
    bool SerachCamera()
    {
        return false;
    }

    bool connectCamera(int chl = 0)
    {
        return isConnect(chl)?false: m_isconnect = true;
    }
    bool disconnectCamera(int chl = 0)
    {
        return isConnect(chl) ? m_isconnect = false : false;
    }
    bool isConnect(int chl = 0)
    {
        return m_isconnect;
    }

    bool startCapture(int chl = 0)
    {
        return isCapture(chl) ? false : m_iscapture = true;
    }
    bool stopCapture(int chl = 0)
    {
        return isCapture(chl) ? m_iscapture = false : false;
    }
    bool isCapture(int chl=0)
    {
        return m_iscapture;
    }

    bool sendCommand()
    {
        return false;
    }
    bool readCommand()
    {
        return false;
    }

    bool currentFrame()
    {
        return false;
    }
private:
    bool m_isconnect = false;
    bool m_iscapture = false;
};
class CoaxPressFactory : public CYCore::CYCameraFactory
{
public:
    CoaxPressFactory()
    {
        setDisplayName(tr("AME"));
        //setIcon(QIcon());
        setCategory("CoaxPress");
        setPriority(0);
        setId("CoaxPress.AME");
    }
    ~CoaxPressFactory()
    {
    }
    CYCore::CYCamera *createCamera()
    {
        return new CoaxPressCamera;
    }
    int SerarchCamera()
    {
        return 2;
    }
};


/*! Constructs the Hello World plugin. Normally plugins don't do anything in
their constructor except for initializing their member variables. The
actual work is done later, in the initialize() and extensionsInitialized()
functions.
*/
CameraCXPPlugin::CameraCXPPlugin()
{
}

/*! Plugins are responsible for deleting objects they created on the heap, and
to unregister objects from the plugin manager that they registered there.
*/
CameraCXPPlugin::~CameraCXPPlugin()
{
}

/*! Initializes the plugin. Returns true on success.
Plugins want to register objects with the plugin manager here.

\a errorMessage can be used to pass an error message to the plugin system,
if there was any.
*/
bool CameraCXPPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    // Create a unique context for our own view, that will be used for the
    // menu entry later.
    Core::Context context("CameraCXP.MainView");

    // Create an action to be triggered by a menu entry
    QAction *helloWorldAction = new QAction(tr("Say \"&Hello World!\""), this);
    connect(helloWorldAction, &QAction::triggered, this, &CameraCXPPlugin::sayHelloWorld);

    // Register the action with the action manager
    Core::Command *command =
        Core::ActionManager::registerAction(
            helloWorldAction, "CameraCXP.CameraCXPAction", context);

    // Create our own menu to place in the Tools menu
    Core::ActionContainer *helloWorldMenu =
        Core::ActionManager::createMenu("CameraCXP.CameraCXPMenu");
    QMenu *menu = helloWorldMenu->menu();
    menu->setTitle(tr("&Hello World"));
    menu->setEnabled(true);

    // Add the Hello World action command to the menu
    helloWorldMenu->addAction(command);

    // Request the Tools menu and add the Hello World menu to it
    Core::ActionContainer *toolsMenu =
        Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    toolsMenu->addMenu(helloWorldMenu);

    // Add a mode with a push button based on BaseMode. Like the BaseView,
    // it will unregister itself from the plugin manager when it is deleted.
    CoaxPressFactory *coaxPressFactory = new CoaxPressFactory;
    addAutoReleasedObject(coaxPressFactory);

    TestProcessor * testProcessor = new TestProcessor;
    addAutoReleasedObject(testProcessor);
    return true;
}

/*! Notification that all extensions that this plugin depends on have been
initialized. The dependencies are defined in the plugins .json(.in) file.

Normally this function is used for things that rely on other plugins to have
added objects to the plugin manager, that implement interfaces that we're
interested in. These objects can now be requested through the
PluginManager.

The HelloWorldPlugin doesn't need things from other plugins, so it does
nothing here.
*/
void CameraCXPPlugin::extensionsInitialized()
{
}

void CameraCXPPlugin::sayHelloWorld()
{
    // When passing 0 for the parent, the message box becomes an
    // application-global modal dialog box
    QMessageBox::information(
        0, tr("Hello World!"), tr("Hello World! Beautiful day today, isn't it?"));
}

} // namespace Internal
} // namespace CameraCXP

