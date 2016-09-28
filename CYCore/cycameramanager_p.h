#ifndef CYCAMERAMANAGER_P_H
#define CYCAMERAMANAGER_P_H

#include <QObject>
namespace CYCore{
namespace Internal{
class CYCameraManagerPrivate : public QObject
{
    Q_OBJECT

public:
    CYCameraManagerPrivate(QObject *parent);
    ~CYCameraManagerPrivate();

private:
    
};
} // namespace Internal
} // namespace CYCore
#endif // CYCAMERAMANAGER_P_H
