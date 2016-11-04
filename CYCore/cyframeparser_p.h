#ifndef CYFRAMEPARSERPRIVATE_H
#define CYFRAMEPARSERPRIVATE_H

#include <QObject>
#include <QThread>
#include <CYCore/cycoreconstants.h>

using namespace CYCore::Constants;
namespace CYCore{
class CYFrameParser;
class CYFrameParserFactory;
namespace Internal{
class CYFrameParserPrivate : public QObject
{
    Q_OBJECT

public:
    CYFrameParserPrivate(CYFrameParserFactory *factory,QObject *parent=0);
    ~CYFrameParserPrivate();
public:
    void do_exec();
signals:
    void sigParseCommit(CYFRAME newframe);
public:
    QThread * m_thr;
    CYFrameParser * m_parent;
    CYFrameParserFactory * m_factory;
    bool      m_enable = false;
    const QWidget * m_contentWidget = nullptr;
private:
    QThread * m_thrParser;
    
};
} // namespace Internal
} // namespace CYCore
#endif // CYFRAMEPARSERPRIVATE_H
