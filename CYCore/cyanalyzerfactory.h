#ifndef CYANALYZERFACTORY_H
#define CYANALYZERFACTORY_H

#include <QObject>
#include "Utils/id.h"
namespace CYCore{
class CYAnalyzer;
class CYAnalyzerFactory : public QObject
{
    Q_OBJECT

public:
    CYAnalyzerFactory(QObject *parent);
    ~CYAnalyzerFactory();
    Utils::Id id() const { return m_id; }
    void setId(Utils::Id id) { m_id = id; }

    virtual CYAnalyzer *createAnalyzer() = 0;
private:
    Utils::Id m_id;
};
}
#endif // CYANALYZERFACTORY_H
