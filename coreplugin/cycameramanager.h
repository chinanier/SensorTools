#ifndef CYCAMERAMANAGER_H
#define CYCAMERAMANAGER_H

#include <QObject>
#include <functional>
#include "Utils/id.h"
#include "coreplugin/core_global.h"
namespace Core {

    namespace Internal {
        class MainWindow;
    }

}
namespace CYCore{
class CYCamera;
class CYFrameParser;
class CYCameraFactory;
namespace Internal{ class CYCameraManagerPrivate; }
class CORE_EXPORT CYCameraManager : public QObject
{
    Q_OBJECT

public:
    typedef QList<CYCameraFactory *> CYCameraFactoryList;

    CYCameraManager(QObject *parent=0);
    ~CYCameraManager();

    static CYCameraManager *instance();
    static CYCameraFactoryList getCameraFactorys(Utils::Id category = Utils::Id());
    static void destroy();

    static int SerachCamera(Utils::Id factoryid = Utils::Id(),uint timeout = 100);

    static bool connectCamera(Utils::Id cameraid, int chl = 0);
    static bool disconnectCamera(Utils::Id cameraid, int chl = 0);
    static bool isConnect(Utils::Id cameraid, int chl = 0);

    static bool startCapture(Utils::Id cameraid, int chl=0);
    static bool stopCapture(Utils::Id cameraid, int chl=0);
    static bool isCapture(Utils::Id cameraid, int chl=0);

    static bool sendCommand();
    static bool readCommand();

    static CYCamera * createCamera(Utils::Id cameraid);
    static CYCamera * currentCamera();
    static Utils::Id currentCameraId();
    static CYCameraFactory * getCameraFactoryFromCameraId(Utils::Id cameraId);
    static CYCamera * getCameraForId(Utils::Id cameraId);
    static Utils::Id getIdForCamera(CYCamera * pCamera);
    
    static void activateCamera(CYCamera *pCamera);
    static void activateCamera(Utils::Id cameraId);

    static bool appendFrameParser(Utils::Id cameraid,CYFrameParser * parser);
    static bool delFrameParser(Utils::Id cameraid, CYFrameParser * parser);
    static QList<CYFrameParser*> getFrameParsers(Utils::Id cameraid,Utils::Id factoryid = Utils::Id());
private:
    static void init();
    static void extensionsInitialized();
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);
    friend class Internal::CYCameraManagerPrivate;
    friend class Core::Internal::MainWindow;
};
} // namespace core
#endif // CYCAMERAMANAGER_H
