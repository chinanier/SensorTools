#ifndef CYFRAMEPARSER_H
#define CYFRAMEPARSER_H

#include <QObject>
#include <CYCore/cycore_global.h>
#include <CYCore/cycoreconstants.h>
using namespace CYCore::Constants;
namespace CYCore {
class CYCamera;
namespace Internal {
    class CYFrameParserPrivate;
};
class CYCORE_EXPORT CYFrameParser : public QObject
{
    Q_OBJECT

public:
    CYFrameParser(QObject *parent);
    virtual ~CYFrameParser();
    virtual void do_exec() =0;

    virtual bool AllocFrameBuffer() = 0;
    virtual bool pushEmptyFrame() = 0;
    virtual bool pushFullFrame(CYFRAME srcframe, CYFRAME & newframe) = 0;
    virtual bool popupEmptyFrame() = 0;
    virtual bool popupFullFrame()= 0;
    bool         newframe(CYFRAME,QObject * pContext);
signals:
    void sigParseCommit(CYFRAME oldframe,CYFRAME newframe);
private:
    Internal::CYFrameParserPrivate * d;
};
}
#endif // CYFRAMEPARSER_H
