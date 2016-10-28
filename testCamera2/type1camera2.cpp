#include "type1camera2.h"

Type1Camera2::Type1Camera2()
{
    connect(this, &CYCamera::sigCompleteFrame, this, &Type1Camera2::slotCompleteFrame);
}
Type1Camera2::~Type1Camera2()
{

}
bool Type1Camera2::SerachCamera()
{
    return false;
}
bool Type1Camera2::connectCamera(int chl )
{
    return isConnect(chl) ? false : m_isconnect = true;
}
bool Type1Camera2::disconnectCamera(int chl )
{
    return isConnect(chl) ? m_isconnect = false : false;
}
bool Type1Camera2::isConnect(int chl )
{
    return m_isconnect;
}

bool Type1Camera2::startCapture(int chl)
{
    return isCapture(chl) ? false : m_iscapture = true;
}
bool Type1Camera2::stopCapture(int chl)
{
    return isCapture(chl) ? m_iscapture = false : false;
}
bool Type1Camera2::isCapture(int chl)
{
    return m_iscapture;
}

bool Type1Camera2::sendCommand()
{
    return false;
}
bool Type1Camera2::readCommand()
{
    return false;
}

bool Type1Camera2::currentFrame()
{
    return false;
}
void Type1Camera2::slotCompleteFrame(CYFRAME frame)
{
    int i = 0;
    i = 1;
    i = 2;
}
