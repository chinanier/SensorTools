#include "type1camera.h"

Type1Camera::Type1Camera()
{
    connect(this, &CYCamera::sigCompleteFrame, this, &Type1Camera::slotCompleteFrame);
}
Type1Camera::~Type1Camera()
{

}
bool Type1Camera::SerachCamera()
{
    return false;
}
bool Type1Camera::connectCamera(int chl )
{
    return isConnect(chl) ? false : m_isconnect = true;
}
bool Type1Camera::disconnectCamera(int chl )
{
    return isConnect(chl) ? m_isconnect = false : false;
}
bool Type1Camera::isConnect(int chl )
{
    return m_isconnect;
}

bool Type1Camera::startCapture(int chl)
{
    return isCapture(chl) ? false : m_iscapture = true;
}
bool Type1Camera::stopCapture(int chl)
{
    return isCapture(chl) ? m_iscapture = false : false;
}
bool Type1Camera::isCapture(int chl)
{
    return m_iscapture;
}

bool Type1Camera::sendCommand()
{
    return false;
}
bool Type1Camera::readCommand()
{
    return false;
}

bool Type1Camera::currentFrame()
{
    return false;
}
void Type1Camera::slotCompleteFrame(CYFRAME frame)
{
    int i = 0;
    i = 1;
    i = 2;
}
