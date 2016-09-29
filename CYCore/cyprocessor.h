#ifndef CYPROCESSOR_H
#define CYPROCESSOR_H

#include <QObject>
#include "cyframeparser.h"
namespace CYCore{
namespace Internal { class CYProcessorPrivate; }
class CYProcessor : public CYFrameParser
{
    Q_OBJECT

public:
    CYProcessor(QObject *parent);
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
