#include "mainwindow.h"
#include "actioncontainer.h"
#include "actionmanager.h"
#include "actionmanager_p.h"
#include "command.h"
#include "QMenu"
using namespace CYCore;
using namespace CYCore::Internal;
MainWindow::MainWindow():
    AppMainWindow(),
    m_coreImpl(new ICore(this))
{
    registerDefaultContainers();
    registerDefaultActions();
}

MainWindow::~MainWindow()
{

}
void MainWindow::registerDefaultContainers()
{
    ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);

    //if (!HostOsInfo::isMacHost()) // System menu bar on Mac
        setMenuBar(menubar->menuBar());
    menubar->appendGroup(Constants::G_FILE);
    menubar->appendGroup(Constants::G_EDIT);
    menubar->appendGroup(Constants::G_VIEW);
    menubar->appendGroup(Constants::G_TOOLS);
    menubar->appendGroup(Constants::G_WINDOW);
    menubar->appendGroup(Constants::G_HELP);

    // File Menu
    ActionContainer *filemenu = ActionManager::createMenu(Constants::M_FILE);
    menubar->addMenu(filemenu, Constants::G_FILE);
    filemenu->menu()->setTitle(tr("&File"));
    filemenu->appendGroup(Constants::G_FILE_NEW);
    filemenu->appendGroup(Constants::G_FILE_OPEN);
    filemenu->appendGroup(Constants::G_FILE_PROJECT);
    filemenu->appendGroup(Constants::G_FILE_SAVE);
    filemenu->appendGroup(Constants::G_FILE_CLOSE);
    filemenu->appendGroup(Constants::G_FILE_PRINT);
    filemenu->appendGroup(Constants::G_FILE_OTHER);
//     //connect(filemenu->menu(), &QMenu::aboutToShow, this, &MainWindow::aboutToShowRecentFiles);
// 
// 
//     // Edit Menu
//     ActionContainer *medit = ActionManager::createMenu(Constants::M_EDIT);
//     menubar->addMenu(medit, Constants::G_EDIT);
//     medit->menu()->setTitle(tr("&Edit"));
//     medit->appendGroup(Constants::G_EDIT_UNDOREDO);
//     medit->appendGroup(Constants::G_EDIT_COPYPASTE);
//     medit->appendGroup(Constants::G_EDIT_SELECTALL);
//     medit->appendGroup(Constants::G_EDIT_ADVANCED);
//     medit->appendGroup(Constants::G_EDIT_FIND);
//     medit->appendGroup(Constants::G_EDIT_OTHER);
// 
//     // Tools Menu
//     ActionContainer *ac = ActionManager::createMenu(Constants::M_TOOLS);
//     menubar->addMenu(ac, Constants::G_TOOLS);
//     ac->menu()->setTitle(tr("&Tools"));
// 
//     // Window Menu
//     ActionContainer *mwindow = ActionManager::createMenu(Constants::M_WINDOW);
//     menubar->addMenu(mwindow, Constants::G_WINDOW);
//     mwindow->menu()->setTitle(tr("&Window"));
//     mwindow->appendGroup(Constants::G_WINDOW_SIZE);
//     mwindow->appendGroup(Constants::G_WINDOW_VIEWS);
//     mwindow->appendGroup(Constants::G_WINDOW_PANES);
//     mwindow->appendGroup(Constants::G_WINDOW_SPLIT);
//     mwindow->appendGroup(Constants::G_WINDOW_NAVIGATE);
//     mwindow->appendGroup(Constants::G_WINDOW_LIST);
//     mwindow->appendGroup(Constants::G_WINDOW_OTHER);
// 
//     // Help Menu
//     ac = ActionManager::createMenu(Constants::M_HELP);
//     menubar->addMenu(ac, Constants::G_HELP);
//     ac->menu()->setTitle(tr("&Help"));
//     ac->appendGroup(Constants::G_HELP_HELP);
//     ac->appendGroup(Constants::G_HELP_SUPPORT);
//     ac->appendGroup(Constants::G_HELP_ABOUT);
//     ac->appendGroup(Constants::G_HELP_UPDATES);
}

void MainWindow::registerDefaultActions()
{
    ActionContainer *mfile = ActionManager::actionContainer(Constants::M_FILE);
//     ActionContainer *medit = ActionManager::actionContainer(Constants::M_EDIT);
//     ActionContainer *mtools = ActionManager::actionContainer(Constants::M_TOOLS);
//     ActionContainer *mwindow = ActionManager::actionContainer(Constants::M_WINDOW);
//     ActionContainer *mhelp = ActionManager::actionContainer(Constants::M_HELP);

    // File menu separators
    mfile->addSeparator(Constants::G_FILE_SAVE);
    mfile->addSeparator(Constants::G_FILE_PRINT);
    mfile->addSeparator(Constants::G_FILE_CLOSE);
    mfile->addSeparator(Constants::G_FILE_OTHER);
    // Edit menu separators
//     medit->addSeparator(Constants::G_EDIT_COPYPASTE);
//     medit->addSeparator(Constants::G_EDIT_SELECTALL);
//     medit->addSeparator(Constants::G_EDIT_FIND);
//     medit->addSeparator(Constants::G_EDIT_ADVANCED);

    // Return to editor shortcut: Note this requires Qt to fix up
    // handling of shortcut overrides in menus, item views, combos....
    m_focusToEditor = new QAction(tr("Return to Editor"), this);
    Command *cmd = ActionManager::registerAction(m_focusToEditor, Constants::S_RETURNTOEDITOR);
    cmd->setDefaultKeySequence(QKeySequence(Qt::Key_Escape));
    /*connect(m_focusToEditor, &QAction::triggered, this, &MainWindow::setFocusToEditor);*/

    // New File Action
     QIcon icon;
//     m_newAction = new QAction(icon, tr("&New File or Project..."), this);
//     cmd = ActionManager::registerAction(m_newAction, Constants::NEW);
//     cmd->setDefaultKeySequence(QKeySequence::New);
//     mfile->addAction(cmd, Constants::G_FILE_NEW);
//     connect(m_newAction, &QAction::triggered, this, [this]() {
//         ICore::showNewItemDialog(tr("New File or Project", "Title of dialog"),
//             IWizardFactory::allWizardFactories(), QString());
//     });
//     connect(ICore::instance(), &ICore::newItemDialogRunningChanged, m_newAction, [this]() {
//         m_newAction->setEnabled(!ICore::isNewItemDialogRunning());
//     });

    // Open Action
//     icon = QIcon::fromTheme(QLatin1String("document-open"), Icons::OPENFILE.icon());
//     m_openAction = new QAction(icon, tr("&Open File or Project..."), this);
//     cmd = ActionManager::registerAction(m_openAction, Constants::OPEN);
//     cmd->setDefaultKeySequence(QKeySequence::Open);
//     mfile->addAction(cmd, Constants::G_FILE_OPEN);
    //connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);

    // Open With Action
    m_openWithAction = new QAction(tr("Open File &With..."), this);
    cmd = ActionManager::registerAction(m_openWithAction, Constants::OPEN_WITH);
    mfile->addAction(cmd, Constants::G_FILE_OPEN);
    //connect(m_openWithAction, &QAction::triggered, this, &MainWindow::openFileWith);

    // File->Recent Files Menu
    ActionContainer *ac = ActionManager::createMenu(Constants::M_FILE_RECENTFILES);
    mfile->addMenu(ac, Constants::G_FILE_OPEN);
    ac->menu()->setTitle(tr("Recent &Files"));
    ac->setOnAllDisabledBehavior(ActionContainer::Show);

    // Save Action
//     icon = QIcon::fromTheme(QLatin1String("document-save"), Icons::SAVEFILE.icon());
     QAction *tmpaction = 0/*new QAction(icon, tr("&Save"), this)*/;
//     tmpaction->setEnabled(false);
//     cmd = ActionManager::registerAction(tmpaction, Constants::SAVE);
//     cmd->setDefaultKeySequence(QKeySequence::Save);
//     cmd->setAttribute(Command::CA_UpdateText);
//     cmd->setDescription(tr("Save"));
//     mfile->addAction(cmd, Constants::G_FILE_SAVE);

    // Save As Action
    icon = QIcon::fromTheme(QLatin1String("document-save-as"));
    tmpaction = new QAction(icon, tr("Save &As..."), this);
    tmpaction->setEnabled(false);
    cmd = ActionManager::registerAction(tmpaction, Constants::SAVEAS);
    cmd->setDefaultKeySequence(QKeySequence(UseMacShortcuts ? tr("Ctrl+Shift+S") : QString()));
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDescription(tr("Save As..."));
    mfile->addAction(cmd, Constants::G_FILE_SAVE);

    // SaveAll Action
    m_saveAllAction = new QAction(tr("Save A&ll"), this);
    cmd = ActionManager::registerAction(m_saveAllAction, Constants::SAVEALL);
    cmd->setDefaultKeySequence(QKeySequence(UseMacShortcuts ? QString() : tr("Ctrl+Shift+S")));
    mfile->addAction(cmd, Constants::G_FILE_SAVE);
    //connect(m_saveAllAction, &QAction::triggered, this, &MainWindow::saveAll);

    // Print Action
    icon = QIcon::fromTheme(QLatin1String("document-print"));
    tmpaction = new QAction(icon, tr("&Print..."), this);
    tmpaction->setEnabled(false);
    cmd = ActionManager::registerAction(tmpaction, Constants::PRINT);
    cmd->setDefaultKeySequence(QKeySequence::Print);
    mfile->addAction(cmd, Constants::G_FILE_PRINT);

    // Exit Action
    icon = QIcon::fromTheme(QLatin1String("application-exit"));
    m_exitAction = new QAction(icon, tr("E&xit"), this);
    m_exitAction->setMenuRole(QAction::QuitRole);
    cmd = ActionManager::registerAction(m_exitAction, Constants::EXIT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Q")));
    mfile->addAction(cmd, Constants::G_FILE_OTHER);
    //connect(m_exitAction, &QAction::triggered, this, &MainWindow::exit);

    // Undo Action
//     icon = QIcon::fromTheme(QLatin1String("edit-undo"), Icons::UNDO.icon());
//     tmpaction = new QAction(icon, tr("&Undo"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::UNDO);
//     cmd->setDefaultKeySequence(QKeySequence::Undo);
//     cmd->setAttribute(Command::CA_UpdateText);
//     cmd->setDescription(tr("Undo"));
//     medit->addAction(cmd, Constants::G_EDIT_UNDOREDO);
//     tmpaction->setEnabled(false);
// 
//     // Redo Action
//     icon = QIcon::fromTheme(QLatin1String("edit-redo"), Icons::REDO.icon());
//     tmpaction = new QAction(icon, tr("&Redo"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::REDO);
//     cmd->setDefaultKeySequence(QKeySequence::Redo);
//     cmd->setAttribute(Command::CA_UpdateText);
//     cmd->setDescription(tr("Redo"));
//     medit->addAction(cmd, Constants::G_EDIT_UNDOREDO);
//     tmpaction->setEnabled(false);
// 
//     // Cut Action
//     icon = QIcon::fromTheme(QLatin1String("edit-cut"), Icons::CUT.icon());
//     tmpaction = new QAction(icon, tr("Cu&t"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::CUT);
//     cmd->setDefaultKeySequence(QKeySequence::Cut);
//     medit->addAction(cmd, Constants::G_EDIT_COPYPASTE);
//     tmpaction->setEnabled(false);
// 
//     // Copy Action
//     icon = QIcon::fromTheme(QLatin1String("edit-copy"), Icons::COPY.icon());
//     tmpaction = new QAction(icon, tr("&Copy"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::COPY);
//     cmd->setDefaultKeySequence(QKeySequence::Copy);
//     medit->addAction(cmd, Constants::G_EDIT_COPYPASTE);
//     tmpaction->setEnabled(false);
// 
//     // Paste Action
//     icon = QIcon::fromTheme(QLatin1String("edit-paste"), Icons::PASTE.icon());
//     tmpaction = new QAction(icon, tr("&Paste"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::PASTE);
//     cmd->setDefaultKeySequence(QKeySequence::Paste);
//     medit->addAction(cmd, Constants::G_EDIT_COPYPASTE);
//     tmpaction->setEnabled(false);
// 
//     // Select All
//     icon = QIcon::fromTheme(QLatin1String("edit-select-all"));
//     tmpaction = new QAction(icon, tr("Select &All"), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::SELECTALL);
//     cmd->setDefaultKeySequence(QKeySequence::SelectAll);
//     medit->addAction(cmd, Constants::G_EDIT_SELECTALL);
//     tmpaction->setEnabled(false);
// 
//     // Goto Action
//     icon = QIcon::fromTheme(QLatin1String("go-jump"));
//     tmpaction = new QAction(icon, tr("&Go to Line..."), this);
//     cmd = ActionManager::registerAction(tmpaction, Constants::GOTO);
//     cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+L")));
//     medit->addAction(cmd, Constants::G_EDIT_OTHER);
//     tmpaction->setEnabled(false);
// 
//     // Options Action
//     mtools->appendGroup(Constants::G_TOOLS_OPTIONS);
//     mtools->addSeparator(Constants::G_TOOLS_OPTIONS);
// 
//     m_optionsAction = new QAction(tr("&Options..."), this);
//     m_optionsAction->setMenuRole(QAction::PreferencesRole);
//     cmd = ActionManager::registerAction(m_optionsAction, Constants::OPTIONS);
//     cmd->setDefaultKeySequence(QKeySequence::Preferences);
//     mtools->addAction(cmd, Constants::G_TOOLS_OPTIONS);
//     //connect(m_optionsAction, &QAction::triggered, this, [this]() { showOptionsDialog(); });
// 
//     mwindow->addSeparator(Constants::G_WINDOW_LIST);
// 
//     if (UseMacShortcuts) {
//         // Minimize Action
//         QAction *minimizeAction = new QAction(tr("Minimize"), this);
//         minimizeAction->setEnabled(false); // actual implementation in WindowSupport
//         cmd = ActionManager::registerAction(minimizeAction, Constants::MINIMIZE_WINDOW);
//         cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+M")));
//         mwindow->addAction(cmd, Constants::G_WINDOW_SIZE);
// 
//         // Zoom Action
//         QAction *zoomAction = new QAction(tr("Zoom"), this);
//         zoomAction->setEnabled(false); // actual implementation in WindowSupport
//         cmd = ActionManager::registerAction(zoomAction, Constants::ZOOM_WINDOW);
//         mwindow->addAction(cmd, Constants::G_WINDOW_SIZE);
//     }
// 
//     // Full Screen Action
//     QAction *toggleFullScreenAction = new QAction(tr("Full Screen"), this);
//     toggleFullScreenAction->setCheckable(!HostOsInfo::isMacHost());
//     toggleFullScreenAction->setEnabled(false); // actual implementation in WindowSupport
//     cmd = ActionManager::registerAction(toggleFullScreenAction, Constants::TOGGLE_FULLSCREEN);
//     cmd->setDefaultKeySequence(QKeySequence(UseMacShortcuts ? tr("Ctrl+Meta+F") : tr("Ctrl+Shift+F11")));
//     if (HostOsInfo::isMacHost())
//         cmd->setAttribute(Command::CA_UpdateText);
//     mwindow->addAction(cmd, Constants::G_WINDOW_SIZE);
// 
//     if (UseMacShortcuts) {
//         mwindow->addSeparator(Constants::G_WINDOW_SIZE);
// 
//         QAction *closeAction = new QAction(tr("Close Window"), this);
//         closeAction->setEnabled(false);
//         cmd = ActionManager::registerAction(closeAction, Constants::CLOSE_WINDOW);
//         cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Meta+W")));
//         mwindow->addAction(cmd, Constants::G_WINDOW_SIZE);
// 
//         mwindow->addSeparator(Constants::G_WINDOW_SIZE);
//     }
// 
//     // Show Sidebar Action
//     m_toggleSideBarAction = new QAction(Icons::TOGGLE_SIDEBAR_TOOLBAR.icon(),
//         QCoreApplication::translate("Core", Constants::TR_SHOW_SIDEBAR),
//         this);
//     m_toggleSideBarAction->setCheckable(true);
//     cmd = ActionManager::registerAction(m_toggleSideBarAction, Constants::TOGGLE_SIDEBAR);
//     cmd->setAttribute(Command::CA_UpdateText);
//     cmd->setDefaultKeySequence(QKeySequence(UseMacShortcuts ? tr("Ctrl+0") : tr("Alt+0")));
//     connect(m_toggleSideBarAction, &QAction::triggered, this, &MainWindow::setSidebarVisible);
//     m_toggleSideBarButton->setDefaultAction(cmd->action());
//     mwindow->addAction(cmd, Constants::G_WINDOW_VIEWS);
//     m_toggleSideBarAction->setEnabled(false);
// 
//     // Show Mode Selector Action
//     m_toggleModeSelectorAction = new QAction(tr("Show Mode Selector"), this);
//     m_toggleModeSelectorAction->setCheckable(true);
//     cmd = ActionManager::registerAction(m_toggleModeSelectorAction, Constants::TOGGLE_MODE_SELECTOR);
//     connect(m_toggleModeSelectorAction, &QAction::triggered,
//         ModeManager::instance(), &ModeManager::setModeSelectorVisible);
//     mwindow->addAction(cmd, Constants::G_WINDOW_VIEWS);
// 
//     // Window->Views
//     ActionContainer *mviews = ActionManager::createMenu(Constants::M_WINDOW_VIEWS);
//     mwindow->addMenu(mviews, Constants::G_WINDOW_VIEWS);
//     mviews->menu()->setTitle(tr("&Views"));
// 
//     // "Help" separators
//     mhelp->addSeparator(Constants::G_HELP_SUPPORT);
//     if (!HostOsInfo::isMacHost())
//         mhelp->addSeparator(Constants::G_HELP_ABOUT);
// 
//     // About IDE Action
//     icon = QIcon::fromTheme(QLatin1String("help-about"));
//     if (HostOsInfo::isMacHost())
//         tmpaction = new QAction(icon, tr("About &Qt Creator"), this); // it's convention not to add dots to the about menu
//     else
//         tmpaction = new QAction(icon, tr("About &Qt Creator..."), this);
//     tmpaction->setMenuRole(QAction::AboutRole);
//     cmd = ActionManager::registerAction(tmpaction, Constants::ABOUT_QTCREATOR);
//     mhelp->addAction(cmd, Constants::G_HELP_ABOUT);
//     tmpaction->setEnabled(true);
//     connect(tmpaction, &QAction::triggered, this, &MainWindow::aboutQtCreator);
// 
//     //About Plugins Action
//     tmpaction = new QAction(tr("About &Plugins..."), this);
//     tmpaction->setMenuRole(QAction::ApplicationSpecificRole);
//     cmd = ActionManager::registerAction(tmpaction, Constants::ABOUT_PLUGINS);
//     mhelp->addAction(cmd, Constants::G_HELP_ABOUT);
//     tmpaction->setEnabled(true);
//     connect(tmpaction, &QAction::triggered, this, &MainWindow::aboutPlugins);
//     // About Qt Action
//     //    tmpaction = new QAction(tr("About &Qt..."), this);
//     //    cmd = ActionManager::registerAction(tmpaction, Constants:: ABOUT_QT);
//     //    mhelp->addAction(cmd, Constants::G_HELP_ABOUT);
//     //    tmpaction->setEnabled(true);
//     //    connect(tmpaction, &QAction::triggered, qApp, &QApplication::aboutQt);
//     // About sep
//     if (!HostOsInfo::isMacHost()) { // doesn't have the "About" actions in the Help menu
//         tmpaction = new QAction(this);
//         tmpaction->setSeparator(true);
//         cmd = ActionManager::registerAction(tmpaction, "QtCreator.Help.Sep.About");
//         mhelp->addAction(cmd, Constants::G_HELP_ABOUT);
//     }
}