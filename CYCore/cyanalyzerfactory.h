#ifndef CYANALYZERFACTORY_H
#define CYANALYZERFACTORY_H

#include <QObject>
#include "coreplugin/id.h"
namespace CYCore{
class CYAnalyzer;
class CYAnalyzerFactory : public QObject
{
    Q_OBJECT

public:
    CYAnalyzerFactory(QObject *parent);
    ~CYAnalyzerFactory();
    Core::Id id() const { return m_id; }
    void setId(Core::Id id) { m_id = id; }

    virtual CYAnalyzer *createAnalyzer() = 0;
private:
    Core::Id m_id;
};
}
#endif // CYANALYZERFACTORY_H
