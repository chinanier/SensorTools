#ifndef CYPROCESSORFACTORY_H
#define CYPROCESSORFACTORY_H

#include <QObject>
#include "coreplugin/id.h"
#include "cycore_global.h"
namespace CYCore{
class CYProcessor;
class CYCORE_EXPORT CYProcessorFactory : public QObject
{
    Q_OBJECT

public:
    CYProcessorFactory(QObject *parent);
    ~CYProcessorFactory();
    Core::Id id() const { return m_id; }
    void setId(Core::Id id) { m_id = id; }

    virtual CYProcessor *createProcessor() = 0;
private:
    Core::Id m_id;
};
}
#endif // CYPROCESSORFACTORY_H
