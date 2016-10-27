#include "cydefaultframeview.h"
#include "cydefaultframeview_p.h"
#include "coreplugin/editormanager/editorview.h"
#include "coreplugin/editormanager/ieditor.h"

#include <QLabel>
#include <QScrollArea>
#include <QImage>
#include <QPixmap>
#include <QTimer>
using namespace Core;
using namespace CYCore;
using namespace CYCore::Internal;
class TestEditor : public IEditor {
public:
    TestEditor()
    {
        QScrollArea * scorll = new QScrollArea;
        m_label = new QLabel;
        //setContext();
        //setContextHelpId();
        m_labelSurfer = new QLabel;
        scorll->setWidget(m_labelSurfer);
        scorll->setWidgetResizable(true);
        setWidget(scorll);
    }
    ~TestEditor()
    {
        
    }
    QWidget *toolBar()
    {
        return m_label;
    }
    IDocument *document()
    {
        return 0;
    }
public:
    QLabel * m_label;
    QLabel * m_labelSurfer;
};

CYDefaultFrameViewPrivate::CYDefaultFrameViewPrivate(QObject *parent)
    : QObject(parent)
{
    m_editor = new TestEditor;
}

CYDefaultFrameViewPrivate::~CYDefaultFrameViewPrivate()
{

}

void CYDefaultFrameViewPrivate::setSurfaceView(EditorView*sfv)
{
    m_sfv = sfv;
    sfv->addEditor(m_editor);
    sfv->setCurrentEditor(m_editor);
    //m_editor->widget()->show();
}
void CYDefaultFrameViewPrivate::toShowFrame(CYFRAME frame)
{
    QImage * image = *(QImage**)frame.s_data;
    m_editor->m_labelSurfer->setPixmap(QPixmap::fromImage(image->scaled(800, 600)));
}

CYDefaultFrameView::CYDefaultFrameView():
    d(new CYDefaultFrameViewPrivate)
{
    AllocFrameBuffer();
}

CYDefaultFrameView::~CYDefaultFrameView()
{
    delete d;
    d = 0;
}
void CYDefaultFrameView::setSurfaceView(EditorView*sfv)
{
    d->setSurfaceView(sfv);
}
void CYDefaultFrameView::doProcess(CYFRAME&frame)
{
    // 处理显示逻辑,非主线程环境?
    QTimer::singleShot(0, d, [this,frame]() {
        d->toShowFrame(frame);
    });
}