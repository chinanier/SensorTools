#ifndef CYCAMERACONFIGMANAGER_H
#define CYCAMERACONFIGMANAGER_H

#include <QObject>
#include <QWidget>
#include <Utils/id.h>
using namespace Utils;
namespace CYCore{
namespace Internal { class CYCameraConfigManagerPrivate; }
class CYCameraConfigManager : public QObject
{
    Q_OBJECT

public:
    CYCameraConfigManager(QObject *parent=0);
    ~CYCameraConfigManager();
    static CYCameraConfigManager * instance();
    static QWidget * createConfigPane(Id mode);
    static QWidget * createSubPane(Id mode,Id camera);

    static QWidget * getConfigPane(Id mode);
    static QWidget * getSubPane(Id mode,Id camera);

    static void extensionsInitialized();
private:
    friend Internal::CYCameraConfigManagerPrivate;
    Internal::CYCameraConfigManagerPrivate * d;
};
}
#endif // CYCAMERACONFIGMANAGER_H
