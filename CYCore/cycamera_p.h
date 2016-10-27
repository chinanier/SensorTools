#ifndef CYCAMERA_P_H
#define CYCAMERA_P_H

#include <QObject>
#include <QThread>
#include <Qlist>
#include <CYCore/cycoreconstants.h>
using namespace CYCore::Constants;
namespace CYCore {
class CYFrameParser;
namespace Internal {
class CYCameraPrivate : public QObject
{
    Q_OBJECT

public:
    CYCameraPrivate(QObject *parent);
    ~CYCameraPrivate();
    QList<CYFrameParser*> m_frameParser;
signals:
    void sigCompleteFrame(CYFRAME);
public slots:
    void slotCompleteFrame(CYFRAME);
    void slotParseCommit(CYFRAME newframe);
    void slotHaveNewFrame(CYFRAME);
private:
    QThread * m_captureThread;
    QThread * m_controlThread;
};
}
}
#endif // CYCAMERA_P_H
