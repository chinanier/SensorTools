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
    virtual void doProcess() = 0;
    virtual void do_exec();

    virtual void AllocFrameBuffer();

    virtual void pushEmptyFrame();
    virtual void pushFullFrame();
    virtual void popupEmptyFrame();
    virtual void popupFullFrame();

private:
    friend class Internal::CYProcessorPrivate;
};
}
#endif // CYPROCESSOR_H
