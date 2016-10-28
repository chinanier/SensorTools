#ifndef TEST2PROCESSORPLUGIN_H
#define TEST2PROCESSORPLUGIN_H

#include <extensionsystem/iplugin.h>

class test2ProcessorPlugin : public ExtensionSystem::IPlugin
{

    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "test2processorplugin.json")

public:
    test2ProcessorPlugin();
    ~test2ProcessorPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();
};
#endif // TEST2PROCESSORPLUGIN_H
