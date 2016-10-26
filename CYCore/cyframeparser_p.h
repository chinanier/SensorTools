#ifndef CYFRAMEPARSERPRIVATE_H
#define CYFRAMEPARSERPRIVATE_H

#include <QObject>
#include <QThread>
namespace CYCore{
namespace Internal{
class CYFrameParserPrivate : public QObject
{
    Q_OBJECT

public:
    CYFrameParserPrivate(QObject *parent);
    ~CYFrameParserPrivate();

public:
    QThread * m_thr;
};
} // namespace Internal
} // namespace CYCore
#endif // CYFRAMEPARSERPRIVATE_H
