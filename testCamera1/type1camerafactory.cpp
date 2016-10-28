#include "type1camerafactory.h"
#include "type1camera.h"
Type1CameraFactory::Type1CameraFactory()
{
    setDisplayName(tr("Camera1"));
    //setIcon(QIcon());
    //setCategory("CoaxPress");
    setCategory("Type1");
    setPriority(0);
    setId("Type1.Camera1");
}

Type1CameraFactory::~Type1CameraFactory()
{

}
CYCore::CYCamera *Type1CameraFactory::createCamera()
{
    return new Type1Camera;
}
int Type1CameraFactory::SerarchCamera()
{
    return 0;
}