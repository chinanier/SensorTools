#include "type2camerafactory2.h"
#include "type2camera2.h"
Type2CameraFactory2::Type2CameraFactory2()
{
    setDisplayName(tr("Camera22"));
    //setIcon(QIcon());
    //setCategory("CoaxPress");
    setCategory("Type2");
    setPriority(2);
    setId("Type2.Camera2");
}

Type2CameraFactory2::~Type2CameraFactory2()
{

}
CYCore::CYCamera *Type2CameraFactory2::createCamera()
{
    return new Type2Camera2;
}
int Type2CameraFactory2::SerarchCamera()
{
    return 0;
}