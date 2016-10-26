#ifndef CYANALYZER_H
#define CYANALYZER_H

#include <QObject>
#include "cyframeparser.h"
namespace CYCore {
namespace Internal{ class CYAnalyzerPrivate; }
class CYAnalyzer : public CYFrameParser
{
    Q_OBJECT

public:
    CYAnalyzer(QObject *parent);
    virtual ~CYAnalyzer();
    virtual void doAnalysis(CYFRAME) = 0;
    virtual void do_exec();

    virtual bool AllocFrameBuffer();
    virtual bool pushEmptyFrame();
    virtual bool pushFullFrame(CYFRAME srcframe, CYFRAME & newframe);
    virtual bool popupEmptyFrame();
    virtual bool popupFullFrame();
private:
    friend class Internal::CYAnalyzerPrivate;
};
}
#endif // CYANALYZER_H
