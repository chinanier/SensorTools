#include "camlinkad0factory.h"
#include "camlinkad0.h"

CamlinkAD0Factory::CamlinkAD0Factory()
{
    setDisplayName(tr("AD0"));
    setName("AD0");
    //setIcon(QIcon());
    //setCategory("CoaxPress");
    setCategory("CameraLink");
    setPriority(0);
    setId("CameraLink.AD0");
}

CamlinkAD0Factory::~CamlinkAD0Factory()
{

}
CYCore::CYCamera *CamlinkAD0Factory::createCamera()
{
    return new CamlinkAD0;
}
int CamlinkAD0Factory::SerarchCamera()
{
    return 1;
}
