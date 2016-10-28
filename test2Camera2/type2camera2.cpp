#include "type2camera2.h"

Type2Camera2::Type2Camera2()
{
    connect(this, &CYCamera::sigCompleteFrame, this, &Type2Camera2::slotCompleteFrame);
}
Type2Camera2::~Type2Camera2()
{

}
bool Type2Camera2::SerachCamera()
{
    return false;
}
bool Type2Camera2::connectCamera(int chl )
{
    return isConnect(chl) ? false : m_isconnect = true;
}
bool Type2Camera2::disconnectCamera(int chl )
{
    return isConnect(chl) ? m_isconnect = false : false;
}
bool Type2Camera2::isConnect(int chl )
{
    return m_isconnect;
}

bool Type2Camera2::startCapture(int chl)
{
    return isCapture(chl) ? false : m_iscapture = true;
}
bool Type2Camera2::stopCapture(int chl)
{
    return isCapture(chl) ? m_iscapture = false : false;
}
bool Type2Camera2::isCapture(int chl)
{
    return m_iscapture;
}

bool Type2Camera2::sendCommand()
{
    return false;
}
bool Type2Camera2::readCommand()
{
    return false;
}

bool Type2Camera2::currentFrame()
{
    return false;
}
void Type2Camera2::slotCompleteFrame(CYFRAME frame)
{
    int i = 0;
    i = 1;
    i = 2;
}
