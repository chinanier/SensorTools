/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "plugindialog.h"
#include <qt_windows.h>
#include <app/app_version.h>
#include <extensionsystem/plugindetailsview.h>
#include <extensionsystem/pluginerrorview.h>
#include <extensionsystem/pluginspec.h>

#include <utils/algorithm.h>
#include <utils/pathchooser.h>
#include <utils/macroexpander.h>
#include <utils/savefile.h>
#include <utils/stringutils.h>
#include <utils/theme/theme.h>
#include <utils/theme/theme_p.h>
#include <utils/fileutils.h>
#include <utils/hostosinfo.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QTemporaryDir>

PluginDialog::PluginDialog(ExtensionSystem::PluginManager *manager)
    : m_view(new ExtensionSystem::PluginView(/*manager,*/ this))
{
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setMargin(0);
    vl->setSpacing(0);
    vl->addWidget(m_view);

    QHBoxLayout *hl = new QHBoxLayout;
    vl->addLayout(hl);
    hl->setMargin(0);
    hl->setSpacing(6);
    m_detailsButton = new QPushButton(tr("Details"), this);
    m_errorDetailsButton = new QPushButton(tr("Error Details"), this);
    m_detailsButton->setEnabled(false);
    m_errorDetailsButton->setEnabled(false);
    hl->addWidget(m_detailsButton);
    hl->addWidget(m_errorDetailsButton);
    hl->addStretch(5);
    resize(650, 300);
    setWindowTitle(tr("Installed Plugins"));

    connect(m_view, SIGNAL(currentPluginChanged(ExtensionSystem::PluginSpec*)),
                this, SLOT(updateButtons()));
    connect(m_view, SIGNAL(pluginActivated(ExtensionSystem::PluginSpec*)),
                this, SLOT(openDetails(ExtensionSystem::PluginSpec*)));
    connect(m_detailsButton, SIGNAL(clicked()), this, SLOT(openDetails()));
    connect(m_errorDetailsButton, SIGNAL(clicked()), this, SLOT(openErrorDetails()));
}

void PluginDialog::updateButtons()
{
    ExtensionSystem::PluginSpec *selectedSpec = m_view->currentPlugin();
    if (selectedSpec) {
        m_detailsButton->setEnabled(true);
        m_errorDetailsButton->setEnabled(selectedSpec->hasError());
    } else {
        m_detailsButton->setEnabled(false);
        m_errorDetailsButton->setEnabled(false);
    }
}


void PluginDialog::openDetails()
{
        openDetails(m_view->currentPlugin());
}

void PluginDialog::openDetails(ExtensionSystem::PluginSpec *spec)
{
    if (!spec)
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Details of %1").arg(spec->name()));
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.setLayout(layout);
    ExtensionSystem::PluginDetailsView *details = new ExtensionSystem::PluginDetailsView(&dialog);
    layout->addWidget(details);
    details->update(spec);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.resize(400, 500);
    dialog.exec();
}

void PluginDialog::openErrorDetails()
{
    ExtensionSystem::PluginSpec *spec = m_view->currentPlugin();
    if (!spec)
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Errors of %1").arg(spec->name()));
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.setLayout(layout);
    ExtensionSystem::PluginErrorView *errors = new ExtensionSystem::PluginErrorView(&dialog);
    layout->addWidget(errors);
    errors->update(spec);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.resize(500, 300);
    dialog.exec();
}
static const char *SHARE_PATH =Utils::HostOsInfo::isMacHost() ? "/../Resources" : "/../share/qtcreator";
static QSettings *createUserSettings()
{
    return new QSettings(QSettings::IniFormat, QSettings::UserScope,
        QLatin1String(CYCore::Constants::IDE_SETTINGSVARIANT_STR),
        QLatin1String("QtCreator"));
}
// taken from utils/fileutils.cpp. We can not use utils here since that depends app_version.h.
static bool copyRecursively(const QString &srcFilePath,
    const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(Utils::FileName::fromString(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach(const QString &fileName, fileNames) {
            const QString newSrcFilePath
                = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    }
    else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}
static inline QSettings *userSettings()
{
    QSettings *settings = createUserSettings();
    const QString fromVariant = QLatin1String(CYCore::Constants::IDE_COPY_SETTINGS_FROM_VARIANT_STR);
    if (fromVariant.isEmpty())
        return settings;

    // Copy old settings to new ones:
    QFileInfo pathFi = QFileInfo(settings->fileName());
    if (pathFi.exists()) // already copied.
        return settings;

    QDir destDir = pathFi.absolutePath();
    if (!destDir.exists())
        destDir.mkpath(pathFi.absolutePath());

    QDir srcDir = destDir;
    srcDir.cdUp();
    if (!srcDir.cd(fromVariant))
        return settings;

    if (srcDir == destDir) // Nothing to copy and no settings yet
        return settings;

    QStringList entries = srcDir.entryList();
    foreach(const QString &file, entries) {
        const QString lowerFile = file.toLower();
        if (lowerFile.startsWith(QLatin1String("profiles.xml"))
            || lowerFile.startsWith(QLatin1String("toolchains.xml"))
            || lowerFile.startsWith(QLatin1String("qtversion.xml"))
            || lowerFile.startsWith(QLatin1String("devices.xml"))
            || lowerFile.startsWith(QLatin1String("debuggers.xml"))
            || lowerFile.startsWith(QLatin1String("qtcreator.")))
            QFile::copy(srcDir.absoluteFilePath(file), destDir.absoluteFilePath(file));
        if (file == QLatin1String("qtcreator"))
            copyRecursively(srcDir.absoluteFilePath(file), destDir.absoluteFilePath(file));
    }

    // Make sure to use the copied settings:
    delete settings;
    return createUserSettings();
}
int main(int argc, char *argv[])
{
    AllocConsole();
    FILE * fp = 0;
    freopen_s(&fp, "conout$", "w", stdout);
    setlocale(LC_ALL, "chs");
    printf("===========Start Debuging=============\n");
    //ExtensionSystem::PluginManager manager;
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseOpenGLES, true);
    //AllocConsole();
    // Manually determine -settingspath command line option
    // We can't use the regular way of the plugin manager, because that needs to parse plugin meta data
    // but the settings path can influence which plugins are enabled
    QString settingsPath;
    QStringList customPluginPaths;
    QStringList arguments = app.arguments(); // adapted arguments list is passed to plugin manager later
    QMutableStringListIterator it(arguments);
    bool testOptionProvided = false;
    QScopedPointer<QTemporaryDir> temporaryCleanSettingsDir;
    if (settingsPath.isEmpty() && testOptionProvided) {
        const QString settingsPathTemplate = QDir::cleanPath(QDir::tempPath()
            + QString::fromLatin1("/qtc-test-settings-XXXXXX"));
        temporaryCleanSettingsDir.reset(new QTemporaryDir(settingsPathTemplate));
        if (!temporaryCleanSettingsDir->isValid())
            return 1;
        settingsPath = temporaryCleanSettingsDir->path();
    }
    if (!settingsPath.isEmpty())
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsPath);

    // Must be done before any QSettings class is created
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope,
        QCoreApplication::applicationDirPath() + QLatin1String(SHARE_PATH));
    QSettings::setDefaultFormat(QSettings::IniFormat);
    // plugin manager takes control of this settings object
    QSettings *settings = userSettings();

    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
        QLatin1String(CYCore::Constants::IDE_SETTINGSVARIANT_STR),
        QLatin1String("QtCreator"));
    ExtensionSystem::PluginManager manager;
    ExtensionSystem::PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.QtCreatorPlugin"));
    ExtensionSystem::PluginManager::setGlobalSettings(globalSettings);
    ExtensionSystem::PluginManager::setSettings(settings);


    ExtensionSystem::PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.CamyuPlugin"));
    Utils::Theme *theme = new Utils::Theme("", qApp);
    Utils::setCreatorTheme(theme);
    QString strpluginPath = QCoreApplication::applicationDirPath() + "/plugins";
    manager.setPluginPaths(QStringList() << strpluginPath);
#if 1
    manager.loadPlugins();
#else
    PluginDialog dialog(&manager);
    dialog.show();
#endif
    // shutdown plugin manager on the exit
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &manager, &ExtensionSystem::PluginManager::shutdown);
    //dialog.show();
    app.exec();
    //FreeConsole();
    FreeConsole();
    return 0;
}
