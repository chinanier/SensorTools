#ifndef CYCAMERAFACTORY_H
#define CYCAMERAFACTORY_H

#include <QObject>

class CYCameraFactory : public QObject
{
    Q_OBJECT

public:
    CYCameraFactory(QObject *parent);
    ~CYCameraFactory();

private:
    
};

#endif // CYCAMERAFACTORY_H
