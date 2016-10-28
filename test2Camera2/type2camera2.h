#ifndef TYPE1CAMERA_H
#define TYPE1CAMERA_H

#include "CYCore/cycamera.h"

class Type2Camera2 : public CYCore::CYCamera
{
    Q_OBJECT

public:
    Type2Camera2();
    ~Type2Camera2();
    bool SerachCamera();
    bool connectCamera(int chl = 0);
    bool disconnectCamera(int chl = 0);
    bool isConnect(int chl = 0);

    bool startCapture(int chl = 0);
    bool stopCapture(int chl = 0);
    bool isCapture(int chl = 0);

    bool sendCommand();
    bool readCommand();

    bool currentFrame();

public slots:
void slotCompleteFrame(CYFRAME frame);
private:
    bool m_isconnect = false;
    bool m_iscapture = false;
};

#endif // TYPE1CAMERA_H
