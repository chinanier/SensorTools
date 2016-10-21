#ifndef CYCAMERAFACTORY_H
#define CYCAMERAFACTORY_H

#include <QObject>
#include "Utils/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYCamera;
class CYCORE_EXPORT CYCameraFactory : public QObject
{
    Q_OBJECT

public:
    CYCameraFactory(QObject *parent);
    ~CYCameraFactory();
    Utils::Id id() const { return m_id; }
    void setId(Utils::Id id) { m_id = id; }
    virtual CYCamera *createCamera() = 0;
private:
    Utils::Id m_id;
};
}
#endif // CYCAMERAFACTORY_H
