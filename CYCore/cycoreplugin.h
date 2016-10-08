#ifndef CYCOREPLUGIN_H
#define CYCOREPLUGIN_H

//#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "../ExtensionSystem/iplugin.h"

namespace CYCore {
     
namespace Internal {
     
class MainWindow;
 
class CYCorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CamyuPlugin" FILE "cycoreplugin.json")
        //Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    CYCorePlugin();
    ~CYCorePlugin();
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();

private:
    bool initializeCalled;
    MainWindow *m_mainWindow;
};
}
}
#endif // CYCOREPLUGIN_H
