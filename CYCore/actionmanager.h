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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

//#include "core_global.h"
#include "coreconstants.h"
#include "icontext.h"
#include "command.h"

#include <QObject>
#include <QList>

QT_BEGIN_NAMESPACE
class QAction;
class QSettings;
class QString;
QT_END_NAMESPACE

namespace CYCore {

class ActionContainer;
class Command;
class Context;

namespace Internal {
class CYCorePlugin;
class MainWindow;
} // Internal

class /*CORE_EXPORT*/ ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();

    static ActionContainer *createMenu(Id id);
    static ActionContainer *createMenuBar(Id id);

    static Command *registerAction(QAction *action, Id id,
                                   const Context &context = Context(Constants::C_GLOBAL),
                                   bool scriptable = false);

    static Command *command(Id id);
    static ActionContainer *actionContainer(Id id);

    static QList<Command *> commands();

    static void unregisterAction(QAction *action, Id id);

    static void setPresentationModeEnabled(bool enabled);
    static bool isPresentationModeEnabled();

signals:
    void commandListChanged();
    void commandAdded(CYCore::Id id);

private:
    ActionManager(QObject *parent = 0);
    ~ActionManager();
    static void saveSettings();
    static void setContext(const Context &context);

    friend class CYCore::Internal::CYCorePlugin; // initialization
    friend class CYCore::Internal::MainWindow; // saving settings and setting context
};

} // namespace Core

#endif // ACTIONMANAGER_H
