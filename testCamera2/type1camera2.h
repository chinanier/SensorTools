#ifndef TYPE1CAMERA2_H
#define TYPE1CAMERA2_H

#include "CYCore/cycamera.h"

class Type1Camera2 : public CYCore::CYCamera
{
    Q_OBJECT

public:
    Type1Camera2();
    ~Type1Camera2();
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

#endif // TYPE1CAMERA2_H
