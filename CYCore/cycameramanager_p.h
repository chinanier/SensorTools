#ifndef CYCAMERAMANAGER_P_H
#define CYCAMERAMANAGER_P_H

#include <QObject>
#include <QMap>
#include "Utils/id.h"
namespace CYCore{
class CYCamera;
namespace Internal{
class CYCameraManagerPrivate : public QObject
{
    Q_OBJECT

public:
    CYCameraManagerPrivate(QObject *parent);
    ~CYCameraManagerPrivate();
    QMap<Utils::Id*, QList<CYCamera*>> m_idOfCameras;
private:
    
};
} // namespace Internal
} // namespace CYCore
#endif // CYCAMERAMANAGER_P_H
