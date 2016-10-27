#ifndef CYANALYZER_H
#define CYANALYZER_H

#include <QObject>
#include "CYCore/cyprocessor.h"
namespace CYCore {
namespace Internal{ class CYAnalyzerPrivate; }
class CYAnalyzer : public CYProcessor
{
    Q_OBJECT

public:
    CYAnalyzer(QObject *parent);
    virtual ~CYAnalyzer();
    virtual void doAnalysis(CYFRAME) = 0;
    virtual void doProcess(CYFRAME&);
private:
    friend class Internal::CYAnalyzerPrivate;
    Internal::CYAnalyzerPrivate * d;
};
}
#endif // CYANALYZER_H
