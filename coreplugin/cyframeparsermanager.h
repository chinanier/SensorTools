#ifndef CYFRAMEPARSERMANAGER_H
#define CYFRAMEPARSERMANAGER_H

#include <QObject>
#include "Utils/id.h"
#include "coreplugin/core_global.h"
namespace Core {

    namespace Internal {
        class MainWindow;
    }

}
namespace CYCore {
class CYFrameParser;
class CYFrameParserFactory;
namespace Internal { class CYFrameParserManagerPrivate; }
class CORE_EXPORT CYFrameParserManager : public QObject
{
    Q_OBJECT

public:
    typedef QList<CYFrameParserFactory *> CYFrameParserFactoryList;
    CYFrameParserManager(QObject *parent=0);
    ~CYFrameParserManager();

    static CYFrameParserManager *instance();
    static CYFrameParserFactoryList getProcessorFactorys(Utils::Id id = Utils::Id());
    static void destroy();
private:
    static void init();
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);
    friend class Internal::CYFrameParserManagerPrivate;
    friend class Core::Internal::MainWindow;
};
}
#endif // CYFRAMEPARSERMANAGER_H
