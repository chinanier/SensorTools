#include "cydefaultframeview.h"
#include "cydefaultframeview_p.h"
#include "coreplugin/editormanager/editorview.h"
#include "coreplugin/editormanager/ieditor.h"
#include "coreplugin/cycameramanager.h"


#include "ExtensionSystem/pluginmanager.h"
#include "cycore/cyframeparserfactory.h"
#include "cycore/cycamera.h"

#include <QLabel>
#include <QScrollArea>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QMenu>
using namespace Core;
using namespace CYCore;
using namespace CYCore::Internal;
class TestEditor : public IEditor {
    Q_OBJECT
public:
    TestEditor()
    {
        QScrollArea * scorll = new QScrollArea;
        m_label = new QLabel;
        //m_label->setContextMenuPolicy(Qt::CustomContextMenu);
        //setContext();
        //setContextHelpId();
        m_labelSurfer = new QLabel;
        // 配置显示窗口
        m_labelSurfer->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(m_labelSurfer, &QWidget::customContextMenuRequested, this, &TestEditor::contextMenuRequested);

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
public slots:
    void contextMenuRequested(QPoint pos)
    {
        QMenu contextMenu;
        QMenu * contextParserMenu = 0;
        QMenu * contextAlyzerMenu = 0;
        // 获取当前的链接的相机id
        CYCamera * pcamera = CYCore::CYCameraManager::currentCamera();
        // 根据id获取当前的处理列表
        QList<CYFrameParserFactory*> parserlist = ExtensionSystem::PluginManager::getObjects<CYFrameParserFactory>();
        foreach(CYFrameParserFactory * p ,parserlist)
        {
            QList<CYFrameParser*> l = pcamera->frameParser(p->id());
            QString parseTypeName;
            QMenu * subContextMenu = 0;
            if (p->type() == CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR)
            {
                parseTypeName = tr("Parse:");
                if (!contextParserMenu)
                {
                    subContextMenu = contextParserMenu = contextMenu.addMenu(tr("Camera Parser"));
                }
            }
            else
            {
                parseTypeName = tr("Alyzer:");
                if (!contextAlyzerMenu)
                {
                    subContextMenu = contextAlyzerMenu = contextMenu.addMenu(tr("Camera Alyzer"));
                }
            }
            // 添加一级菜单,添加处理器接口
            QString strAction = tr("Add ") + parseTypeName + p->displayName() + "...";
            subContextMenu->addAction(strAction, this, []() {});
            if (l.size()>0)
            {
                QMenu * pmenu = 0;
                for (int i=0;i<l.size();i++)
                {
                    // 添加二级菜单
                    CYFrameParser*fp = l.at(i);
                    QString strAction;
                    QString submenuName = p->displayName() + "." + Id::fromSetting(i).toString();
                    pmenu = subContextMenu->addMenu(submenuName);
                    // 是否启动
                    if (fp->isEnabled())
                    {
                        strAction = tr("Stop ") + parseTypeName + p->displayName() + "...";
                        pmenu->addAction(strAction, this, []() {
                        });
                    }
                    else
                    {
                        strAction = tr("Start ") + parseTypeName + p->displayName() + "...";
                        strAction += submenuName + "...";
                        pmenu->addAction(strAction, this, []() {
                        });
                    }
                    // 是否显示,通过EditManager获取,或这保存到处理器中?
                    strAction = tr("Show ")+ submenuName + "...";
                    pmenu->addAction(strAction, this, []() {
                    });
                    strAction = tr("Hide ")+submenuName + "...";
                    pmenu->addAction(strAction, this, []() {
                    });
                    // 从处理链路中移除此解析器
                    strAction = tr("Remove ")+ submenuName + "...";
                    pmenu->addAction(strAction, this, []() {
                    });
                }
            }
        }
        if (!parserlist.isEmpty())
        {
            contextMenu.exec(/*mapToGlobal(pos)*/QCursor::pos());
        }
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
    if (frame.s_color == PIX_COLOR_QIMAGE)
    {
        QImage * image = *(QImage**)frame.s_data;
        m_editor->m_labelSurfer->setPixmap(QPixmap::fromImage(image->scaled(800, 600)));
    }
    else
    {
        QImage image((uchar*)frame.s_data,frame.s_width,frame.s_height,QImage::Format_Indexed8);
        m_editor->m_labelSurfer->setPixmap(QPixmap::fromImage(image));
    }
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
#include "cydefaultframeview.moc"