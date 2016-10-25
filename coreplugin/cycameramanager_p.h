#ifndef CYCAMERAMANAGER_P_H
#define CYCAMERAMANAGER_P_H

#include <QObject>
#include <QMap>
#include <QHash>
#include "Utils/id.h"
namespace CYCore{
class CYCamera;
class CYCameraFactory;
namespace Internal{
class CYCameraManagerPrivate : public QObject
{
    Q_OBJECT

public:
    typedef QList<CYCameraFactory *> CYCameraFactoryList;

    CYCameraManagerPrivate(QObject *parent);
    ~CYCameraManagerPrivate();
    QMap<Utils::Id, QList<CYCamera*>> m_idOfCameras; // id is factoryid
    QHash<Utils::Id, CYCameraFactory*> m_idOfFactorys;
    QMap<QString, CYCameraFactoryList> m_categoryOfFactory;
    CYCameraFactoryList m_cameraFactoryList;
public:
    CYCamera * getCameraForCameraID(Utils::Id cameraid);
    Utils::Id  getFactoryIdForCameraID(Utils::Id cameraid);
private:
    
};
} // namespace Internal
} // namespace CYCore
#endif // CYCAMERAMANAGER_P_H
