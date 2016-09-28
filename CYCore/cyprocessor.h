#ifndef CYPROCESSOR_H
#define CYPROCESSOR_H

#include <QObject>

class CYProcessor : public QObject
{
    Q_OBJECT

public:
    CYProcessor(QObject *parent);
    ~CYProcessor();

private:
    
};

#endif // CYPROCESSOR_H
