#ifndef TYPE1CAMERAFACTORY_H
#define TYPE1CAMERAFACTORY_H

#include <QObject>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>

class Type1CameraFactory2 : public CYCore::CYCameraFactory
{
    Q_OBJECT

public:
    Type1CameraFactory2();
    ~Type1CameraFactory2();
    CYCore::CYCamera *createCamera();
    int SerarchCamera();
};

#endif // TYPE1CAMERAFACTORY_H
