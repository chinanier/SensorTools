#include "cycameracategorymode.h"
#include <extensionsystem/iplugin.h>

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
/*!  A mode with a push button based on BaseMode.  */
QWidget *createModeWindow(const Id &mode)
{
    auto documentAndRightPane = new MiniSplitter;
    documentAndRightPane->addWidget(new EditorManagerPlaceHolder(documentAndRightPane, mode));
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

CYCameraCategoryMode::CYCameraCategoryMode(Id m_mode, QObject *parent)
    : IMode(parent)
{
    m_widget = createModeWindow(m_mode);
    setWidget(m_widget);
    setContext(Core::Context(m_mode));
    setDisplayName(tr(/*"CoaxPress"*/"CYCameraCategory"));
    //setIcon(QIcon());
    setPriority(0);
    setId(m_mode);
    setContextHelpId(QString());
}

CYCameraCategoryMode::~CYCameraCategoryMode()
{
    if (m_widget)
    {
        delete m_widget;
        m_widget = 0;
    }
}
