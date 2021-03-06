#ifndef CYCAMERA_H
#define CYCAMERA_H

#include <QObject>
#include <CYCore/cycore_global.h>
#include <CYCore/cycoreconstants.h>
#include <Utils/id.h>

using namespace Utils;
using namespace CYCore::Constants;
namespace CYCore {
class CYFrameParser;
class CYCameraFactory;
namespace Internal{ class CYCameraPrivate; }
class CYCORE_EXPORT CYCamera : public QObject
{
    Q_OBJECT

public:
    CYCamera(QObject *parent=0);
    ~CYCamera();
    CYCameraFactory * factory();
    void setFactory(CYCameraFactory*);

    virtual bool SerachCamera() = 0;

    bool connectCameraHelper(int chl);
    bool disconnectCameraHelper(int chl);
    virtual bool connectCamera(int chl=0) = 0;
    virtual bool disconnectCamera(int chl = 0) = 0;
    virtual bool isConnect(int chl=0) = 0;

    bool startCaptureHelper(int chl);
    bool stopCaptureHelper(int chl);
    virtual bool startCapture(int chl = 0) = 0;
    virtual bool stopCapture(int chl = 0) = 0;
    virtual bool isCapture(int chl=0) = 0;

    virtual bool sendCommand() = 0;
    virtual bool readCommand() = 0;

    virtual bool currentFrame() = 0;

    bool addFrameParser(CYFrameParser * newNode, CYFrameParser * before = 0);
    bool delFrameParser(CYFrameParser * parser);
    QList<CYFrameParser*> frameParser(Id factoryid = Id());

    

private:
    friend class Internal::CYCameraPrivate;
    Internal::CYCameraPrivate * d;
    
signals :
    void sigHaveNewFrame(CYFRAME);
    void sigCompleteFrame(CYFRAME);
    void sigConnectChange(int chl);
    void sigCaptureChange(int chl);
public slots:

};
}
Q_DECLARE_METATYPE(CYFRAME);
#endif // CYCAMERA_H
