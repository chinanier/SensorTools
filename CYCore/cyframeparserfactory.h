#ifndef CYFRAMEPARSERFACTORY_H
#define CYFRAMEPARSERFACTORY_H

#include <QObject>
#include <QIcon>
#include <QString>

#include "Utils/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYFrameParser;
class CYCORE_EXPORT CYFrameParserFactory : public QObject
{
    Q_OBJECT

public:
    typedef enum CYFRAMEPARSERTYPE
    {
        CYFRAMEPARSER_PROCESSOR,
        CYFRAMEPARSER_ANALYZER
    };
    CYFrameParserFactory(QObject *parent=0);
    ~CYFrameParserFactory();
    Utils::Id id() const { return m_id; }
    CYFRAMEPARSERTYPE type() const {return m_type;}
    QString displayName() const { return m_displayName; }
    QIcon icon() const { return m_icon; }

    void setId(Utils::Id id) { m_id = id; }
    void setType(CYFRAMEPARSERTYPE type) { m_type = type; }
    void setDisplayName(const QString &displayName) { m_displayName = displayName; }
    void setIcon(const QIcon &icon) { m_icon = icon; }

    virtual CYFrameParser *createFrameParser() = 0;
private:
    Utils::Id m_id;
    CYFRAMEPARSERTYPE m_type;
    QIcon m_icon;
    QString m_displayName;
};
}
#endif // CYFRAMEPARSERFACTORY_H
