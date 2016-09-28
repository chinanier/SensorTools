#ifndef CYANALYZERFACTORY_H
#define CYANALYZERFACTORY_H

#include <QObject>

class CYAnalyzerFactory : public QObject
{
    Q_OBJECT

public:
    CYAnalyzerFactory(QObject *parent);
    ~CYAnalyzerFactory();

private:
    
};

#endif // CYANALYZERFACTORY_H
