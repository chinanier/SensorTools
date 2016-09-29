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
    virtual void doAnalysis() = 0;
    virtual void do_exec();

    virtual void AllocFrameBuffer();
    virtual void pushEmptyFrame();
    virtual void pushFullFrame();
    virtual void popupEmptyFrame();
    virtual void popupFullFrame();
private:
    friend class Internal::CYAnalyzerPrivate;
};
}
#endif // CYANALYZER_H
