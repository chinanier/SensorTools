#ifndef CYFRAMEPARSERMANAGERPRIVATE_H
#define CYFRAMEPARSERMANAGERPRIVATE_H

#include <QObject>
#include <QHash>

#include <Utils/id.h>

using namespace Utils;
namespace CYCore {
class CYFrameParserFactory;
namespace Internal {
class CYFrameParserManagerPrivate : public QObject
{
	Q_OBJECT

public:
    CYFrameParserManagerPrivate(QObject *parent);
	~CYFrameParserManagerPrivate();
	QHash<Id, CYFrameParserFactory*> m_factorysOfid;
private:

};
}
}
#endif // CYFRAMEPARSERMANAGERPRIVATE_H
