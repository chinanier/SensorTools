#ifndef CYDEFAULTFRAMEVIEW_H
#define CYDEFAULTFRAMEVIEW_H

#include <CYCore/cyprocessor.h>

namespace Core {
    namespace Internal {
        class EditorView;
    }
}
using namespace Core::Internal;
namespace CYCore{
    namespace Internal {
        class CYDefaultFrameViewPrivate;
    }
class CYDefaultFrameView : public CYProcessor
{
    Q_OBJECT

public:
    CYDefaultFrameView();
    ~CYDefaultFrameView();
    virtual void doProcess(CYFRAME&);

    void setSurfaceView(EditorView*sfv);
private:
    friend class Internal::CYDefaultFrameViewPrivate;
    Internal::CYDefaultFrameViewPrivate * d;
};
}
#endif // CYDEFAULTFRAMEVIEW_H
