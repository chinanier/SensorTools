#include "type2camerafactory.h"
#include "type2camera.h"
Type2CameraFactory::Type2CameraFactory()
{
    setDisplayName(tr("Camera21"));
    //setIcon(QIcon());
    //setCategory("CoaxPress");
    setCategory("Type2");
    setPriority(0);
    setId("Type2.Camera1");
}

Type2CameraFactory::~Type2CameraFactory()
{

}
CYCore::CYCamera *Type2CameraFactory::createCamera()
{
    return new Type2Camera;
}
int Type2CameraFactory::SerarchCamera()
{
    return 0;
}