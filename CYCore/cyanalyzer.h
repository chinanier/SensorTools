#ifndef CYANALYZER_H
#define CYANALYZER_H

#include <QObject>
#include "CYCore/cyprocessor.h"
namespace CYCore {
class CYFrameParserFactory;
namespace Internal{ class CYAnalyzerPrivate; }
class CYAnalyzer : public CYProcessor
{
    Q_OBJECT

public:
    CYAnalyzer(CYFrameParserFactory * factory=0,QObject *parent=0);
    virtual ~CYAnalyzer();
    virtual void doAnalysis(CYFRAME) = 0;
    virtual void doProcess(CYFRAME&);
private:
    friend class Internal::CYAnalyzerPrivate;
    Internal::CYAnalyzerPrivate * d;
};
}
#endif // CYANALYZER_H
