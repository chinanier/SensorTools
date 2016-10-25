#ifndef CYCAMERAFACTORY_H
#define CYCAMERAFACTORY_H

#include <QObject>
#include <QIcon>
#include "Utils/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYCamera;
class CYCORE_EXPORT CYCameraFactory : public QObject
{
    Q_OBJECT

public:
    CYCameraFactory(QObject *parent=0);
    virtual ~CYCameraFactory();
    QString displayName() const { return m_displayName; }
    Utils::Id id() const { return m_id; }
    QString category() const { return m_category; }
    QString name() const { return m_name; }
    int priority() const { return m_priority; }
    QIcon icon() const { return m_icon; }

    void setId(Utils::Id id) { m_id = id; }
    void setName(QString name) { m_name = name; }
    void setCategory(QString category) { m_category = category; }
    void setIcon(const QIcon &icon) { m_icon = icon; }
    void setPriority(int priority) { m_priority = priority; }
    void setDisplayName(const QString &displayName) { m_displayName = displayName; }
    virtual CYCamera *createCamera() = 0;
    virtual int SerarchCamera() = 0;
private:
    QString m_displayName;
    Utils::Id m_id;     ///<生产类id,用于区别在同一类别下不同的生产类的驱动和版本
    QString m_name;     ///<标志生产类的名称
    QString m_category; ///<类别,对该生产类进行分类描述
    int m_priority;
    QIcon m_icon;
};
}
#endif // CYCAMERAFACTORY_H
