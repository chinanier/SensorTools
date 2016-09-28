#ifndef PLUGIN1PLUGIN_H
#define PLUGIN1PLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "../ExtensionSystem/iplugin.h"
class Plugin1Plugin :public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "plugin1plugin.json")
    //Q_INTERFACES(ExtensionSystem::IPlugin)

public:
    Plugin1Plugin();
    virtual ~Plugin1Plugin();
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();

private:
    bool initializeCalled;
};

#endif // PLUGIN1PLUGIN_H
