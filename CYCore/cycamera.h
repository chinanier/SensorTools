#ifndef CYCAMERA_H
#define CYCAMERA_H

#include <QObject>
namespace CYCore {
namespace Internal{ class CYCameraPrivate; }
class CYCamera : public QObject
{
    Q_OBJECT

public:
    CYCamera(QObject *parent);
    ~CYCamera();
    virtual bool SerachCamera() = 0;
    virtual bool connectCamera() = 0;
    virtual bool disconnectCamera() = 0;

    virtual bool startCapture() = 0;
    virtual bool stopCapture() = 0;

    virtual bool sendCommand() = 0;
    virtual bool readCommand() = 0;

    virtual bool currentFrame() = 0;
private:
    friend class Internal::CYCameraPrivate;
    Internal::CYCameraPrivate * d;
signals :
    void sigHaveNewFrame();
public slots:

};
}
#endif // CYCAMERA_H
