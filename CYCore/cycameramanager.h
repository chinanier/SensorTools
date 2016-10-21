#ifndef CYCAMERAMANAGER_H
#define CYCAMERAMANAGER_H

#include <QObject>
#include "Utils/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYCamera;
class CYCameraFactory;
namespace Internal{ class CYCameraManagerPrivate; }
class CYCORE_EXPORT CYCameraManager : public QObject
{
    Q_OBJECT

public:
    typedef QList<CYCameraFactory *> CYCameraFactoryList;

    CYCameraManager(QObject *parent);
    ~CYCameraManager();

    static CYCameraManager *instance();

    static bool SerachCamera(Utils::Id id = Utils::Id());

    static bool connectCamera(Utils::Id id = Utils::Id(), int chl = 0);
    static bool disconnectCamera(Utils::Id id = Utils::Id(), int chl = 0);

    static bool startCapture();
    static bool stopCapture();

    static bool sendCommand();
    static bool readCommand();

    static CYCamera * createCamera(Utils::Id id = Utils::Id());
    static CYCamera * currentCamera();
    static void activateCamera(CYCamera *pCamera);
private:
    friend class Internal::CYCameraManagerPrivate;
};
} // namespace core
#endif // CYCAMERAMANAGER_H
