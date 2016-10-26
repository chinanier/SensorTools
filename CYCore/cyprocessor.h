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
    virtual void doProcess(CYFRAME) = 0;
    virtual void do_exec();

    virtual bool AllocFrameBuffer();

    virtual bool pushEmptyFrame();
    virtual bool pushFullFrame(CYFRAME srcframe,CYFRAME & newframe);
    virtual bool popupEmptyFrame();
    virtual bool popupFullFrame();

private:
    friend class Internal::CYProcessorPrivate;
    Internal::CYProcessorPrivate * d;
};
}
#endif // CYPROCESSOR_H
