#include "type2camera.h"

Type2Camera::Type2Camera()
{
    connect(this, &CYCamera::sigCompleteFrame, this, &Type2Camera::slotCompleteFrame);
}
Type2Camera::~Type2Camera()
{

}
bool Type2Camera::SerachCamera()
{
    return false;
}
bool Type2Camera::connectCamera(int chl )
{
    return isConnect(chl) ? false : m_isconnect = true;
}
bool Type2Camera::disconnectCamera(int chl )
{
    return isConnect(chl) ? m_isconnect = false : false;
}
bool Type2Camera::isConnect(int chl )
{
    return m_isconnect;
}

bool Type2Camera::startCapture(int chl)
{
    return isCapture(chl) ? false : m_iscapture = true;
}
bool Type2Camera::stopCapture(int chl)
{
    return isCapture(chl) ? m_iscapture = false : false;
}
bool Type2Camera::isCapture(int chl)
{
    return m_iscapture;
}

bool Type2Camera::sendCommand()
{
    return false;
}
bool Type2Camera::readCommand()
{
    return false;
}

bool Type2Camera::currentFrame()
{
    return false;
}
void Type2Camera::slotCompleteFrame(CYFRAME frame)
{
    int i = 0;
    i = 1;
    i = 2;
}
