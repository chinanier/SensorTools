#ifndef CYPROCESSOR_P_H
#define CYPROCESSOR_P_H

#include <QObject>
#include <QThread>
#include <cycoreconstants.h>
using namespace CYCore::Constants;
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
    
public:
    QThread * m_processorThread;
    QList<CYFRAME> m_emptybuffer;
    QList<CYFRAME> m_fullbuffer;
    void * m_pMemory;
};
}
}
#endif // CYPROCESSOR_P_H
