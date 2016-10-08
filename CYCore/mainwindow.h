#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "../utils/appmainwindow.h"
#include "../utils/dropsupport.h"
#include "icontext.h"
#include "icore.h"
#include <QMap>
#include <QColor>

#include <functional>

namespace CYCore {
     
namespace Internal {
     
class MainWindow : public Utils::AppMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    ICore *m_coreImpl;

    // actions
    QAction *m_focusToEditor;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_openWithAction;
    QAction *m_saveAllAction;
    QAction *m_exitAction;
    QAction *m_optionsAction;
    QAction *m_toggleSideBarAction;
    QAction *m_toggleModeSelectorAction;
    QAction *m_themeAction;
    void registerDefaultContainers();
    void registerDefaultActions();
};
}
}
#endif // MAINWINDOW_H
