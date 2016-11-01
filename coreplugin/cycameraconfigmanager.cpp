#include "cycameraconfigmanager.h"
#include "CamyuPropertyEdit/camyupropertyedit.h"
#include "rightpane.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/cycameramanager.h>

#include <cycore/cycamera.h>

#include <QHash>
using namespace Core;
using namespace CAMYUPROPEDIT;
namespace CYCore{
static CYCameraConfigManager * m_current = 0;
namespace Internal {
class CYCameraConfigManagerPrivate {
public:
    QHash<Id, RightPaneWidget*> m_modeOfPane;
    QHash<Id, QHash<Id, CamyuPropertyEdit*>> m_modeOfProperPane;
};
}
CYCameraConfigManager::CYCameraConfigManager(QObject *parent)
    : QObject(parent),
    d(new Internal::CYCameraConfigManagerPrivate)
{
    if (!m_current)
    {
        m_current = this;
    }
}

CYCameraConfigManager::~CYCameraConfigManager()
{
    if (d)
    {
        delete d;
        d = 0;
    }
}
void CYCameraConfigManager::extensionsInitialized()
{
    connect(EditorManager::instance(), &EditorManager::currentSubEditorChanged, CYCameraConfigManager::instance(), [](Id cameraId) {
        RightPaneWidget * rp = RightPaneWidget::instance();
        if (rp)
        {
            QWidget * wid = CYCameraConfigManager::getSubPane(ModeManager::currentMode(), cameraId);
            if (!wid)
            {
                wid = CYCameraConfigManager::createSubPane(ModeManager::currentMode(), cameraId);
            }
            rp->activeWidget(wid);
        }
    });
}

CYCameraConfigManager * CYCameraConfigManager::instance()
{
    return m_current;
}
QWidget * CYCameraConfigManager::createConfigPane(Id mode)
{
    if (CYCameraConfigManager::instance()->d->m_modeOfPane.contains(mode))
    {
        return CYCameraConfigManager::instance()->d->m_modeOfPane[mode];
    }
    RightPaneWidget * widget = new RightPaneWidget;
    CYCameraConfigManager::instance()->d->m_modeOfPane.insert(mode, widget);
    return widget;
}
QWidget * CYCameraConfigManager::createSubPane(Id mode, Id camera)
{
    if (!CYCameraConfigManager::instance()->d->m_modeOfPane.contains(mode))
    {
        // 还没有创建mode
        return 0;
    }
    QHash<Id, CamyuPropertyEdit*> & subWidge = CYCameraConfigManager::instance()->d->m_modeOfProperPane[mode];
    if (subWidge.contains(camera))
    {
        return subWidge[camera];
    }
    CamyuPropertyEdit * widget = new CamyuPropertyEdit;
    widget->bindCamera(CYCameraManager::getCameraForId(camera));
    widget->bindCameraFactory(CYCameraManager::getCameraFactoryFromCameraId(camera));
    subWidge.insert(camera, widget);
    return widget;
}

QWidget * CYCameraConfigManager::getConfigPane(Id mode)
{
    if (CYCameraConfigManager::instance()->d->m_modeOfPane.contains(mode))
    {
        return CYCameraConfigManager::instance()->d->m_modeOfPane[mode];
    }
    return 0;
}
QWidget * CYCameraConfigManager::getSubPane(Id mode, Id camera)
{
    if (CYCameraConfigManager::instance()->d->m_modeOfPane.contains(mode))
    {
        if (CYCameraConfigManager::instance()->d->m_modeOfProperPane[mode].contains(camera))
        {
            return CYCameraConfigManager::instance()->d->m_modeOfProperPane[mode][camera];
        }
    }
    return 0;
}
}
