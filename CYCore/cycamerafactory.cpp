#include "cycamera.h"
#include "cycamerafactory.h"
using namespace CYCore;
CYCameraFactory::CYCameraFactory(QObject *parent)
    : QObject(parent)
{

}

CYCameraFactory::~CYCameraFactory()
{

}
CYCamera *CYCameraFactory::createCamera_helper()
{
    CYCamera * pcamera = createCamera();
    if (pcamera)
    {
        pcamera->setFactory(this);
    }
    return pcamera;
}