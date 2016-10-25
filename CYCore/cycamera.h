#ifndef CYCAMERA_H
#define CYCAMERA_H

#include <QObject>
#include <CYCore/cycore_global.h>
namespace CYCore {
class CYFrameParser;
namespace Internal{ class CYCameraPrivate; }
class CYCORE_EXPORT CYCamera : public QObject
{
    Q_OBJECT

public:
    CYCamera(QObject *parent=0);
    ~CYCamera();

    virtual bool SerachCamera() = 0;

    virtual bool connectCamera(int chl=0) = 0;
    virtual bool disconnectCamera(int chl = 0) = 0;
    virtual bool isConnect(int chl=0) = 0;

    virtual bool startCapture(int chl = 0) = 0;
    virtual bool stopCapture(int chl = 0) = 0;
    virtual bool isCapture(int chl=0) = 0;

    virtual bool sendCommand() = 0;
    virtual bool readCommand() = 0;

    virtual bool currentFrame() = 0;

    bool addFrameParser(CYFrameParser * newNode, CYFrameParser * before = 0);
private:
    friend class Internal::CYCameraPrivate;
    Internal::CYCameraPrivate * d;
    
signals :
    void sigHaveNewFrame();
public slots:

};
}
#endif // CYCAMERA_H
