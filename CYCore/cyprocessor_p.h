#ifndef CYPROCESSOR_P_H
#define CYPROCESSOR_P_H

#include <QObject>
#include <QThread>
namespace CYCore {
namespace Internal {
class CYProcessorPrivate : public QObject
{
    Q_OBJECT

public:
    CYProcessorPrivate(QObject *parent);
    ~CYProcessorPrivate();
signals:
    void sigPushEmptyFrame();
    void sigPushFullFrame();
    void sigPopupEmptyFrame();
    void sigPopupFullFrame();
public slots:
    
private:
    QThread * m_processorThread;
    void * m_pFrame;
};
}
}
#endif // CYPROCESSOR_P_H
