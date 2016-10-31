#ifndef CAMLINKAD0_H
#define CAMLINKAD0_H

#include "CYCore/cycamera.h"
class CamlinkAD0Private;
class CamlinkAD0 : public CYCore::CYCamera
{
    Q_OBJECT

public:
    CamlinkAD0();
    ~CamlinkAD0();
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
    friend class CamlinkAD0Private;
    bool m_isconnect = false;
    bool m_iscapture = false;
    CamlinkAD0Private * d;
};
#endif // CAMLINKAD0_H
