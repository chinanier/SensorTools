#ifndef CYANALYZER_P_H
#define CYANALYZER_P_H

#include <QObject>
namespace CYCore {
namespace Internal {
class CYAnalyzerPrivate : public QObject
{
    Q_OBJECT

public:
    CYAnalyzerPrivate(QObject *parent);
    ~CYAnalyzerPrivate();

private:

};
}
}
#endif // CYANALYZER_P_H
