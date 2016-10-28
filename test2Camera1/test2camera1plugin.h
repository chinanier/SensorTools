#ifndef TEST2CAMERA1PLUGIN_H
#define TEST2CAMERA1PLUGIN_H

#include <extensionsystem/iplugin.h>

class test2Camera1Plugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "test2camera1plugin.json")

public:
    test2Camera1Plugin();
    ~test2Camera1Plugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};

#endif // TEST2CAMERA1PLUGIN_H
