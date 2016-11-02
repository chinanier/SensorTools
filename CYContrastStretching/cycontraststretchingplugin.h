#ifndef CYCONTRASTSTRETCHINGPLUGIN_H
#define CYCONTRASTSTRETCHINGPLUGIN_H

#include <extensionsystem/iplugin.h>

class CYContrastStretchingPlugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "cycontraststretchingplugin.json")

public:
    CYContrastStretchingPlugin();
    ~CYContrastStretchingPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};

#endif // CYCONTRASTSTRETCHINGPLUGIN_H
