#ifndef TYPE1CAMERAFACTORY_H
#define TYPE1CAMERAFACTORY_H

#include <QObject>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>

class Type2CameraFactory : public CYCore::CYCameraFactory
{
    Q_OBJECT

public:
    Type2CameraFactory();
    ~Type2CameraFactory();
    CYCore::CYCamera *createCamera();
    int SerarchCamera();
};

#endif // TYPE1CAMERAFACTORY_H
