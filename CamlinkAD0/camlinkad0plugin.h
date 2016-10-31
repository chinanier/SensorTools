#ifndef CAMLINKAD0PLUGIN_H
#define CAMLINKAD0PLUGIN_H

#include <extensionsystem/iplugin.h>

class CamlinkAD0Plugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "camlinkad0plugin.json")

public:
    CamlinkAD0Plugin();
    ~CamlinkAD0Plugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);
    void extensionsInitialized();
};

#endif // CAMLINKAD0PLUGIN_H
