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


#include "Utils/algorithm.h"
#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QStackedWidget>
#include <QToolButton>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QDir>
#include <qt_windows.h>

using namespace Core;
using namespace CYCore;

namespace CameraCXP {
namespace Internal {

class TestFrameParser : public CYCore::CYProcessor
{
public:
    TestFrameParser()
    {
        AllocFrameBuffer();
    }
    virtual void doProcess(CYFRAME &frame)
    {
        int i = 0;
        i = i;
        qDebug() << "There is TestFrameParser" << this << "==> id:" << frame.s_id << "buffer:" << frame.s_data;
        //QThread::sleep(1);
    }
};

class Test2FrameParser : public CYCore::CYProcessor
{
public:
    Test2FrameParser()
    {
        AllocFrameBuffer();
    }
    virtual void doProcess(CYFRAME &frame)
    {
        int i = 0;
        i = i;
        qDebug() << "There is Test2FrameParser"<<this<<"==> id:" << frame.s_id << "buffer:" << frame.s_data;
        //QThread::sleep(1);
    }
};
class Test2Processor : public CYCore::CYFrameParserFactory {
public:
    Test2Processor()
    {
        setId("TEST2PROCESSOR");
        setDisplayName("Test2Processor");
        setType(CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR);
    }
    ~Test2Processor()
    {

    }
    CYFrameParser *createFrameParser() {
        return new Test2FrameParser;
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

static QStringList jpgFiles(const QStringList &jpgPaths)
{
    QStringList pluginFiles;
    QStringList searchPaths = jpgPaths;
    while (!searchPaths.isEmpty()) {
        const QDir dir(searchPaths.takeFirst());
        const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        const QStringList absoluteFilePaths = Utils::transform(files, &QFileInfo::absoluteFilePath);
        pluginFiles += Utils::filtered(absoluteFilePaths, [](const QString &path) {
            return true;
        });
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        searchPaths += Utils::transform(dirs, &QFileInfo::absoluteFilePath);
    }
    return pluginFiles;
}
class CoaxPressCamera : public CYCore::CYCamera
{
    Q_OBJECT

public:
    CoaxPressCamera()
    {
        connect(this, &CYCamera::sigCompleteFrame, this, &CoaxPressCamera::slotCompleteFrame);
        QString strpluginPath = QCoreApplication::applicationDirPath() + "/testimage";
        QStringList jpglist = jpgFiles(QStringList() << strpluginPath);
        m_maxjpgcount = jpglist.size();
        for (int i=0;i<jpglist.size();i++)
        {
            m_pImage[i] = new QImage(jpglist[i]);
        }
    }
    ~CoaxPressCamera()
    {
        
    }
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
        m_timerCaptureFrame = startTimer(100);
        return isCapture(chl) ? false : m_iscapture = true;
    }
    bool stopCapture(int chl = 0)
    {
        killTimer(m_timerCaptureFrame);
        m_timerCaptureFrame = 0;
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
    void timerEvent(QTimerEvent *event)
    {
        //killTimer(m_timerCaptureFrame);
        if (event->timerId() == m_timerCaptureFrame)
        {
            m_tstart = GetTickCount();
            CYFRAME frame = { 0 };
            frame.s_id = m_captureCount++;
            frame.s_data = &m_pImage[frame.s_id % m_maxjpgcount];
            frame.s_length = sizeof(QImage**);
            qDebug() << "=============================================";
            qDebug() << "There is user Provd==> id:" << frame.s_id << "buffer:" << frame.s_data;
            emit sigHaveNewFrame(frame);
        }
        //m_timerCaptureFrame = 0;
    }
public slots:
    void slotCompleteFrame(CYFRAME frame)
    {
        int i = 0;
        i = 1;
        i = 2;
        m_tstop = GetTickCount();
        qDebug() << "There is " << this << "::slotCompleteFrame ==> id:" << frame.s_id << "buffer:" << frame.s_data;
        qDebug() << "totle time:" << (m_tstop - m_tstart)/1000.0<<" ms";
        qDebug() << "=============================================";
    }
private:
    bool m_isconnect = false;
    bool m_iscapture = false;
    int  m_timerCaptureFrame = 0;
    uint m_captureCount = 0;
    uint m_captureData[100] = { 0 };
    QImage * m_pImage[100] = { 0 };
    uint     m_maxjpgcount = 0;
    DWORD m_tstart;
    DWORD m_tstop;
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
        return 1;
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

    Test2Processor * test2Processor = new Test2Processor;
    addAutoReleasedObject(test2Processor);
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

#include "cameracxp.moc"