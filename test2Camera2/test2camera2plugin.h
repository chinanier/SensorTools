#ifndef TEST2CAMERA2PLUGIN_H
#define TEST2CAMERA2PLUGIN_H

#include <extensionsystem/iplugin.h>

class test2Camera2Plugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "test2camera2plugin.json")

public:
    test2Camera2Plugin();
    ~test2Camera2Plugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};
#endif // TEST2CAMERA2PLUGIN_H
