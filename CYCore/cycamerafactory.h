#ifndef CYCAMERAFACTORY_H
#define CYCAMERAFACTORY_H

#include <QObject>
#include "id.h"
namespace CYCore{
class CYCamera;
class CYCameraFactory : public QObject
{
    Q_OBJECT

public:
    CYCameraFactory(QObject *parent);
    ~CYCameraFactory();
    Id id() const { return m_id; }
    void setId(Id id) { m_id = id; }
    virtual CYCamera *createCamera() = 0;
private:
    Id m_id;
};
}
#endif // CYCAMERAFACTORY_H
