#ifndef TESTPROCESSORPLUGIN_H
#define TESTPROCESSORPLUGIN_H

#include <extensionsystem/iplugin.h>

class testProcessorPlugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "testprocessorplugin.json")

public:
    testProcessorPlugin();
    ~testProcessorPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};

#endif // TESTPROCESSORPLUGIN_H
