#ifndef CYANALYZERFACTORY_H
#define CYANALYZERFACTORY_H

#include <QObject>
#include "id.h"
namespace CYCore{
class CYAnalyzer;
class CYAnalyzerFactory : public QObject
{
    Q_OBJECT

public:
    CYAnalyzerFactory(QObject *parent);
    ~CYAnalyzerFactory();
    Id id() const { return m_id; }
    void setId(Id id) { m_id = id; }

    virtual CYAnalyzer *createAnalyzer() = 0;
private:
    Id m_id;
};
}
#endif // CYANALYZERFACTORY_H
