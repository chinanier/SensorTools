#ifndef CYFRAMEPARSER_H
#define CYFRAMEPARSER_H

#include <QObject>
namespace CYCore {
class CYFrameParser : public QObject
{
    Q_OBJECT

public:
    CYFrameParser(QObject *parent);
    virtual ~CYFrameParser();
    virtual void do_exec() =0;

    virtual void AllocFrameBuffer() = 0;
    virtual void pushEmptyFrame() = 0;
    virtual void pushFullFrame() = 0;
    virtual void popupEmptyFrame() = 0;
    virtual void popupFullFrame()= 0;
private:

};
}
#endif // CYFRAMEPARSER_H
