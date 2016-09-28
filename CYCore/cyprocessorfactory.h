#ifndef CYPROCESSORFACTORY_H
#define CYPROCESSORFACTORY_H

#include <QObject>

class CYProcessorFactory : public QObject
{
    Q_OBJECT

public:
    CYProcessorFactory(QObject *parent);
    ~CYProcessorFactory();

private:
    
};

#endif // CYPROCESSORFACTORY_H
