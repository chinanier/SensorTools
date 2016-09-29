#ifndef CYCAMERAMANAGER_H
#define CYCAMERAMANAGER_H

#include <QObject>
#include "id.h"
namespace CYCore{
class CYCamera;
class CYCameraFactory;
namespace Internal{ class CYCameraManagerPrivate; }
class CYCameraManager : public QObject
{
    Q_OBJECT

public:
    typedef QList<CYCameraFactory *> CYCameraFactoryList;

    CYCameraManager(QObject *parent);
    ~CYCameraManager();

    static CYCameraManager *instance();

    static bool SerachCamera(Id id = Id());

    static bool connectCamera(Id id = Id(), int chl = 0);
    static bool disconnectCamera(Id id = Id(), int chl = 0);

    static bool startCapture();
    static bool stopCapture();

    static bool sendCommand();
    static bool readCommand();

    static CYCamera * createCamera(Id id = Id());
    static CYCamera * currentCamera();
    static void activateCamera(CYCamera *pCamera);
private:
    friend class Internal::CYCameraManagerPrivate;
};
} // namespace core
#endif // CYCAMERAMANAGER_H
