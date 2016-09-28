#ifndef CYCAMERAMANAGER_H
#define CYCAMERAMANAGER_H

#include <QObject>
#include "cycameramanager_p.h"
namespace CYCore{
class CYCameraManager : public QObject
{
    Q_OBJECT

public:
    CYCameraManager(QObject *parent);
    ~CYCameraManager();

private:
    friend class Internal::CYCameraManagerPrivate;
};
} // namespace core
#endif // CYCAMERAMANAGER_H
