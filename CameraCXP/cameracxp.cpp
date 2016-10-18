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

#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QStackedWidget>
#include <QToolButton>
#include <QMessageBox>

using namespace Core;
namespace CameraCXP {
namespace Internal {

/*!  A mode with a push button based on BaseMode.  */
QWidget *createModeWindow(const Core::Id &mode)
{
    auto documentAndRightPane = new MiniSplitter;
    documentAndRightPane->addWidget(new EditorManagerPlaceHolder(documentAndRightPane));
    documentAndRightPane->addWidget(new RightPanePlaceHolder(mode));
    documentAndRightPane->setStretchFactor(0, 1);
    documentAndRightPane->setStretchFactor(1, 0);

    auto centralEditorWidget = new QWidget;
    auto centralLayout = new QHBoxLayout(centralEditorWidget);
    centralEditorWidget->setLayout(centralLayout);
    centralLayout->setMargin(0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(documentAndRightPane);
    centralLayout->setStretch(0, 1);
    centralLayout->setStretch(1, 0);

    // Right-side window with editor, output etc.
    auto mainWindowSplitter = new MiniSplitter;
    mainWindowSplitter->addWidget(centralEditorWidget);
    mainWindowSplitter->addWidget(new OutputPanePlaceHolder(mode, mainWindowSplitter));
    mainWindowSplitter->setStretchFactor(0, 10);
    mainWindowSplitter->setStretchFactor(1, 0);
    mainWindowSplitter->setOrientation(Qt::Vertical);

    // Navigation and right-side window.
    auto splitter = new MiniSplitter;
    //splitter->setFocusProxy(mainWindow->centralWidgetStack());
    splitter->addWidget(new NavigationWidgetPlaceHolder(mode));
    splitter->addWidget(mainWindowSplitter);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    //splitter->setObjectName(QLatin1String("DebugModeWidget"));
    //mainWindow->setCentralWidget(centralEditorWidget);

    return splitter;
}
class HelloMode : public Core::IMode
{
public:
    HelloMode()
    {
        /*setWidget(new QPushButton(tr("CameraCXP PushButton!")));*/
        setWidget(createModeWindow("CameraCXP.CameraCXPMode"));
        setContext(Core::Context("CameraCXP.MainView"));
        setDisplayName(tr("CoaxPress"));
        //setIcon(QIcon());
        setPriority(0);
        setId("CameraCXP.CameraCXPMode");
        setContextHelpId(QString());
        //setDisplayName(tr("CameraCXP"));
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
    Core::IMode *helloMode = new HelloMode;
    addAutoReleasedObject(helloMode);

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

