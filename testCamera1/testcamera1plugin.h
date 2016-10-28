#ifndef TESTCAMERA1PLUGIN_H
#define TESTCAMERA1PLUGIN_H

#include <extensionsystem/iplugin.h>

class testCamera1Plugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "testcamera1plugin.json")

public:
    testCamera1Plugin();
    ~testCamera1Plugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};

#endif // TESTCAMERA1PLUGIN_H
