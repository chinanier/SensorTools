#ifndef CYCAMERA_P_H
#define CYCAMERA_P_H

#include <QObject>
#include <QThread>
#include <Qlist>
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
private:
    void parseCommit(CYFrameParser*pframeParser);
signals:
    void sigParseCommit();
public slots:
    // 处理完毕，用于那种需要自定义内存的处理通知已经处理好了一帧
    void slotParseCommit();
    void slotHaveNewFrame();
private:
    QThread * m_captureThread;
    QThread * m_controlThread;
};
}
}
#endif // CYCAMERA_P_H
