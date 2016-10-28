#ifndef TESTCAMERA2PLUGIN_H
#define TESTCAMERA2PLUGIN_H

#include <extensionsystem/iplugin.h>

class testCamera2Plugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "testcamera2plugin.json")

public:
    testCamera2Plugin();
    ~testCamera2Plugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};

#endif // TESTCAMERA2PLUGIN_H
