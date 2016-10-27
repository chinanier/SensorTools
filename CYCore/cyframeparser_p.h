#ifndef CYFRAMEPARSERPRIVATE_H
#define CYFRAMEPARSERPRIVATE_H

#include <QObject>
#include <QThread>
#include <CYCore/cycoreconstants.h>

using namespace CYCore::Constants;
namespace CYCore{
class CYFrameParser;
namespace Internal{
class CYFrameParserPrivate : public QObject
{
    Q_OBJECT

public:
    CYFrameParserPrivate(QObject *parent=0);
    ~CYFrameParserPrivate();
public:
    void do_exec();
signals:
    void sigParseCommit(CYFRAME newframe);
public:
    QThread * m_thr;
    CYFrameParser * m_parent;
private:
    QThread * m_thrParser;
    
};
} // namespace Internal
} // namespace CYCore
#endif // CYFRAMEPARSERPRIVATE_H
