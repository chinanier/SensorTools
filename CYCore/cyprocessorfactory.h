#ifndef CYPROCESSORFACTORY_H
#define CYPROCESSORFACTORY_H

#include <QObject>
#include "id.h"
namespace CYCore{
class CYProcessor;
class CYProcessorFactory : public QObject
{
    Q_OBJECT

public:
    CYProcessorFactory(QObject *parent);
    ~CYProcessorFactory();
    Id id() const { return m_id; }
    void setId(Id id) { m_id = id; }

    virtual CYProcessor *createProcessor() = 0;
private:
    Id m_id;
};
}
#endif // CYPROCESSORFACTORY_H
