#ifndef TYPE1CAMERAFACTORY_H
#define TYPE1CAMERAFACTORY_H

#include <QObject>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>

class Type1CameraFactory : public CYCore::CYCameraFactory
{
    Q_OBJECT

public:
    Type1CameraFactory();
    ~Type1CameraFactory();
    CYCore::CYCamera *createCamera();
    int SerarchCamera();
};

#endif // TYPE1CAMERAFACTORY_H
