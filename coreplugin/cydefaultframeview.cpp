#include "cydefaultframeview.h"
#include "cydefaultframeview_p.h"

#include "coreplugin/editormanager/editormanager.h"
#include "coreplugin/editormanager/editorview.h"
#include "coreplugin/editormanager/ieditor.h"
#include "coreplugin/cycameramanager.h"


#include "ExtensionSystem/pluginmanager.h"
#include "cycore/cyframeparserfactory.h"
#include "cycore/cycamera.h"
#include "cycore/cyglimagearea.h"
#include <QOpenGLTexture>

#include "Utils/id.h"

#include <QLabel>
#include <QScrollArea>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QMdiSubWindow>

using namespace Core;
using namespace Utils;
using namespace CYCore;
using namespace CYCore::Internal;
class TestEditor : public IEditor {
    Q_OBJECT
public:
    TestEditor()
    {
        //QScrollArea * scorll = new QScrollArea;
        m_label = new QLabel;
        //m_label->setContextMenuPolicy(Qt::CustomContextMenu);
        //setContext();
        //setContextHelpId();
        m_labelSurfer = new CYGLImageArea;
        // ������ʾ����
        m_labelSurfer->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(m_labelSurfer, &QWidget::customContextMenuRequested, this, &TestEditor::contextMenuRequested);

        //scorll->setWidget(m_labelSurfer);
        //scorll->setWidgetResizable(true);
        setWidget(/*scorll*/m_labelSurfer);
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
        // ��ȡ��ǰ�����ӵ����id
        Id cameraid = CYCore::CYCameraManager::currentCameraId();
        // ����id��ȡ��ǰ�Ĵ����б�
        QList<CYFrameParserFactory*> parserlist = ExtensionSystem::PluginManager::getObjects<CYFrameParserFactory>();
        // ��ӻ�������
        
        if (!CYCameraManager::isConnect(cameraid))
        {
            contextMenu.addAction("Connect", this, [this, cameraid]() {
                CYCameraManager::connectCamera(cameraid);
                QMessageBox::information(
                    0, tr("Connect!"), tr("Connect Camera Success!"));
                // ֪ͨeditManager��������
            });
        }
        else
        {
            contextMenu.addAction("disConnect", this, [this, cameraid]() {
                CYCameraManager::disconnectCamera(cameraid);
                QMessageBox::information(
                    0, tr("Connect!"), tr("disConnect Camera Success!"));
                // ֪ͨeditManager���ٴ���
            });
            // ���ӿ�ʼ�ɼ����Ҽ��˵�
            if (!CYCameraManager::isCapture(cameraid))
            {
                contextMenu.addAction("StartCapture", this, [this, cameraid]() {
                    CYCameraManager::startCapture(cameraid);
                    QMessageBox::information(
                        0, tr("StartCapture!"), tr("StartCapture Camera Success!"));
                    // ֪ͨeditManager��������
                });
            }
            else
            {
                contextMenu.addAction("StopCapture", this, [this, cameraid]() {
                    CYCameraManager::stopCapture(cameraid);
                    QMessageBox::information(
                        0, tr("StopCapture!"), tr("StopCapture Camera Success!"));
                    // ֪ͨeditManager��������
                });
            }
        }
        foreach(CYFrameParserFactory * p ,parserlist)
        {
            QList<CYFrameParser*> l = CYCameraManager::getFrameParsers(cameraid, p->id());
            QString parseTypeName;
            QMenu * subContextMenu = contextMenu.addMenu(p->displayName());
#if 0
            if (p->type() == CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR)
            {
                parseTypeName = tr("Parse:");
                if (!contextParserMenu)
                {
                    contextParserMenu = contextMenu.addMenu(tr("Camera Parser"));
                }
                subContextMenu = contextParserMenu;
            }
            else
            {
                parseTypeName = tr("Alyzer:");
                if (!contextAlyzerMenu)
                {
                    contextAlyzerMenu = contextMenu.addMenu(tr("Camera Alyzer"));
                }
                subContextMenu = contextAlyzerMenu;
            }
#endif
            // ���һ���˵�,��Ӵ������ӿ�
            QString strAction = tr("Add ") + parseTypeName + p->displayName() + "...";
            subContextMenu->addAction(strAction, this, [this,cameraid,p]() {
                EditorView * pedit = EditorManager::createProcessEditorView(cameraid, p->id());
                // �������
                CYFrameParser * fpser = p->createFrameParser_helper();
                // ��ϵͳ���������չʾ�Ĵ��������Ҫ��ָ����id��������һ��������,�����Ϳ�����ϵͳ���е��ȴ�����
                CYCameraManager::appendFrameParser(cameraid, fpser);
                fpser->setContentWidget(pedit);
                // ������ô���
                if (fpser->widget())
                {
                    pedit->setWidget(fpser->widget());
                }
                auto func = [pedit,this]() {
                    //pedit->close();
                    QMdiSubWindow * pmdisub = qobject_cast<QMdiSubWindow*>(pedit->parent());
                    if (pmdisub)
                    {
                        disconnect(pmdisub, &QObject::destroyed, this, 0);
                        pmdisub->close();
                    }
                };
                // ���༭�����ڵ������¼��봦�������а�
                connect(pedit->parent() ? pedit->parent() : pedit, &QObject::destroyed, this, [this,cameraid, fpser, func]() {
                    int i = 19;
                    i = 0;
                    disconnect(fpser, &CYFrameParser::sigAboutToDestroyed, this,0);
                    CYCameraManager::delFrameParser(cameraid, fpser);
                });
                connect(fpser, &CYFrameParser::sigAboutToDestroyed,this, func);
            });
            if (l.size()>0)
            {
                QMenu * pmenu = 0;
                for (int i=0;i<l.size();i++)
                {
                    // ��Ӷ����˵�
                    CYFrameParser*fp = l.at(i);
                    QString strAction;
                    QString submenuName = p->displayName() + "." + Id::fromSetting(i).toString();
                    pmenu = subContextMenu->addMenu(submenuName);
                    // �Ƿ�����
                    if (fp->isEnabled())
                    {
                        strAction = tr("Stop ") + parseTypeName + submenuName + "...";
                        pmenu->addAction(strAction, this, [fp]() {
                            fp->setEnabled(false);
                        });
                    }
                    else
                    {
                        strAction = tr("Start ") + parseTypeName + submenuName + "...";
                        pmenu->addAction(strAction, this, [fp]() {
                            fp->setEnabled(true);
                        });
                    }
                    const QWidget * contentwidget = fp->contentWidget();
                    if (contentwidget)
                    {
                        QMdiSubWindow * pmdisub = qobject_cast<QMdiSubWindow*>(contentwidget->parent());
                        if (pmdisub)
                        {
                            if (pmdisub->isHidden())
                            {
                                // �Ƿ���ʾ,ͨ��EditManager��ȡ,���Ᵽ�浽��������?
                                strAction = tr("Show ") + submenuName + "...";
                                pmenu->addAction(strAction, this, [pmdisub]() {
                                    pmdisub->show();
                                });
                            }
                            else
                            {
                                strAction = tr("Hide ") + submenuName + "...";
                                pmenu->addAction(strAction, this, [pmdisub]() {
                                    pmdisub->hide();
                                });
                            }
                        }
                    }
                    // �Ӵ�����·���Ƴ��˽�����
                    strAction = tr("Remove ")+ submenuName + "...";
                    pmenu->addAction(strAction, this, [cameraid,fp]() {
                        // ����
                        CYCameraManager::delFrameParser(cameraid, fp);
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
    //QLabel * m_labelSurfer;
    CYGLImageArea * m_labelSurfer;
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
        //m_editor->m_labelSurfer->setPixmap(QPixmap::fromImage(image->scaled(800, 600)));
        m_editor->m_labelSurfer->setData(image->width(), image->height(), QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image->constBits());
    }
    else
    {
        //QImage image((uchar*)frame.s_data,frame.s_width,frame.s_height,QImage::Format_Indexed8);
        //m_editor->m_labelSurfer->setPixmap(QPixmap::fromImage(image));
        m_editor->m_labelSurfer->setData(frame.s_width, frame.s_height, QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, frame.s_data);
    }
}

CYDefaultFrameView::CYDefaultFrameView():
    d(new CYDefaultFrameViewPrivate)
{
    AllocFrameBuffer();
    setEnabled(true);
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
    // ������ʾ�߼�,�����̻߳���?
    QTimer::singleShot(0, d, [this,frame]() {
        d->toShowFrame(frame);
    });
}
#include "cydefaultframeview.moc"