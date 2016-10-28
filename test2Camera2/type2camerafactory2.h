#ifndef TYPE1CAMERAFACTORY_H
#define TYPE1CAMERAFACTORY_H

#include <QObject>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>

class Type2CameraFactory2 : public CYCore::CYCameraFactory
{
    Q_OBJECT

public:
    Type2CameraFactory2();
    ~Type2CameraFactory2();
    CYCore::CYCamera *createCamera();
    int SerarchCamera();
};

#endif // TYPE1CAMERAFACTORY_H
