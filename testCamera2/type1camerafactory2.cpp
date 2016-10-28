#include "type1camerafactory2.h"
#include "type1camera2.h"
Type1CameraFactory2::Type1CameraFactory2()
{
    setDisplayName(tr("Camera2"));
    //setIcon(QIcon());
    //setCategory("CoaxPress");
    setCategory("Type1");
    setPriority(0);
    setId("Type1.Camera2");
}

Type1CameraFactory2::~Type1CameraFactory2()
{

}
CYCore::CYCamera *Type1CameraFactory2::createCamera()
{
    return new Type1Camera2;
}
int Type1CameraFactory2::SerarchCamera()
{
    return 0;
}