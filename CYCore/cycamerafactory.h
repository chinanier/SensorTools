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
    Utils::Id m_id;     ///<������id,����������ͬһ����²�ͬ��������������Ͱ汾
    QString m_name;     ///<��־�����������
    QString m_category; ///<���,�Ը���������з�������
    int m_priority;
    QIcon m_icon;
};
}
#endif // CYCAMERAFACTORY_H
