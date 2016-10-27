#ifndef CYPROCESSOR_H
#define CYPROCESSOR_H

#include <QObject>
#include <CYCore/cycore_global.h>
#include "cyframeparser.h"
namespace CYCore{
namespace Internal { class CYProcessorPrivate; }
class CYCORE_EXPORT CYProcessor : public CYFrameParser
{
    Q_OBJECT

public:
    CYProcessor(QObject *parent=0);
    virtual ~CYProcessor();
    virtual void doProcess(CYFRAME&) = 0;
    virtual void do_exec(CYFRAME&frame);

    virtual bool AllocFrameBuffer();
    virtual bool pushEmptyFrame(CYFRAME frame);
    virtual bool pushFullFrame(CYFRAME frame);
    virtual bool popupEmptyFrame(CYFRAME & frame);
    virtual bool popupFullFrame(CYFRAME & frame);

private:
    friend class Internal::CYProcessorPrivate;
    Internal::CYProcessorPrivate * d;
};
}
#endif // CYPROCESSOR_H
