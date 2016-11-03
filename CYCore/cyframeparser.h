#ifndef CYFRAMEPARSER_H
#define CYFRAMEPARSER_H

#include <QObject>
#include <CYCore/cycore_global.h>
#include <CYCore/cycoreconstants.h>
#include <QWidget>
using namespace CYCore::Constants;
namespace CYCore {
class CYCamera;
class CYFrameParserFactory;
namespace Internal {
    class CYFrameParserPrivate;
};
class CYCORE_EXPORT CYFrameParser : public QObject
{
    Q_OBJECT

public:
    CYFrameParser(CYFrameParserFactory*factory=0,QObject *parent=0);
    virtual ~CYFrameParser();
    virtual void do_exec(CYFRAME&) =0;

    virtual bool AllocFrameBuffer() = 0;
    virtual bool pushEmptyFrame(CYFRAME frame) = 0;
    virtual bool pushFullFrame(CYFRAME frame) = 0;
    virtual bool popupEmptyFrame(CYFRAME & frame) = 0;
    virtual bool popupFullFrame(CYFRAME & frame)= 0;
    bool         newFrame(CYFRAME);
    void         completeFrame(CYFRAME frame);
    void setFactory(CYFrameParserFactory * factory);
    CYFrameParserFactory * factory();
    // ʹ�ܿ���

    virtual void setEnabled(bool);
    bool isEnabled() const;

    // �������ý���
    void applySettings();
    // ����ͼ - ����״̬,����ͼ״̬����ʾ״̬(����ŵ�)
    virtual QWidget * widget() = 0;    // �Զ������
    
signals:
    void sigFrameCopyCommit(CYFRAME frame);
    void sigParseCommit(CYFRAME newframe);
    
private:
    Internal::CYFrameParserPrivate * d;
};
}
#endif // CYFRAMEPARSER_H
