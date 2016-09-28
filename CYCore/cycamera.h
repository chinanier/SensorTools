#ifndef CYCAMERA_H
#define CYCAMERA_H

#include <QObject>

class CYCamera : public QObject
{
    Q_OBJECT

public:
    CYCamera(QObject *parent);
    ~CYCamera();

private:
    
};

#endif // CYCAMERA_H
