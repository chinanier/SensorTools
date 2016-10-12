#ifndef CYCAMERAFACTORY_H
#define CYCAMERAFACTORY_H

#include <QObject>
#include "coreplugin/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYCamera;
class CYCORE_EXPORT CYCameraFactory : public QObject
{
    Q_OBJECT

public:
    CYCameraFactory(QObject *parent);
    ~CYCameraFactory();
    Core::Id id() const { return m_id; }
    void setId(Core::Id id) { m_id = id; }
    virtual CYCamera *createCamera() = 0;
private:
    Core::Id m_id;
};
}
#endif // CYCAMERAFACTORY_H
