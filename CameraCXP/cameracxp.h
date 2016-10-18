#ifndef CAMERACXP_H
#define CAMERACXP_H

#include <extensionsystem/iplugin.h>

namespace CameraCXP {
namespace Internal {

class CameraCXPPlugin
    : public ExtensionSystem::IPlugin
{
    Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "CameraCXP.json")

public:
    CameraCXPPlugin();
    ~CameraCXPPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();

private:
    void sayHelloWorld();
};

} // namespace Internal
} // namespace CameraCXP

#endif // CAMERACXP_H
