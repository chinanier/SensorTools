#ifndef CYPROCESSOR_P_H
#define CYPROCESSOR_P_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <cycoreconstants.h>
using namespace CYCore::Constants;
namespace CYCore {
namespace Internal {
class CYProcessorPrivate : public QObject
{
    Q_OBJECT

public:
    CYProcessorPrivate(QObject *parent=0);
    ~CYProcessorPrivate();
public:
    bool AllocFrameBuffer();
    bool pushEmptyFrame(CYFRAME frame);
    bool pushFullFrame(CYFRAME frame);
    bool popupEmptyFrame(CYFRAME & frame);
    bool popupFullFrame(CYFRAME & frame);
    bool isFull(EBufferType);
signals:
    
public slots:
    
public:
    QList<CYFRAME> m_emptybuffer;
    QList<CYFRAME> m_fullbuffer;
    uchar * m_pMemory = nullptr;
};
}
}
#endif // CYPROCESSOR_P_H
