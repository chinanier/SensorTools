#ifndef CYDEFAULTFRAMEVIEWPRIVATE_H
#define CYDEFAULTFRAMEVIEWPRIVATE_H

#include <QObject>
#include <CYCore/cycoreconstants.h>
using namespace CYCore::Constants;
class TestEditor;
namespace Core {
    namespace Internal {
        class EditorView;
    }
}
namespace CYCore{
namespace Internal{
class CYDefaultFrameViewPrivate : public QObject
{
    Q_OBJECT

public:
    CYDefaultFrameViewPrivate(QObject *parent=0);
    ~CYDefaultFrameViewPrivate();

    void setSurfaceView(Core::Internal::EditorView*sfv);
    void toShowFrame(CYFRAME frame);
private:
    Core::Internal::EditorView * m_sfv = nullptr;
    TestEditor * m_editor;
};
}
}
#endif // CYDEFAULTFRAMEVIEWPRIVATE_H
