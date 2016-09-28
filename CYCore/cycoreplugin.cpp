#include "cycore.h"

#include <QtCore/QtPlugin>
#include "cycoreplugin.h"


CYCorePlugin::CYCorePlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void CYCorePlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (initialized)
        return;

    initialized = true;
}

bool CYCorePlugin::isInitialized() const
{
    return initialized;
}

QWidget *CYCorePlugin::createWidget(QWidget *parent)
{
    return new CYCore(parent);
}

QString CYCorePlugin::name() const
{
    return "CYCore";
}

QString CYCorePlugin::group() const
{
    return "My Plugins";
}

QIcon CYCorePlugin::icon() const
{
    return QIcon();
}

QString CYCorePlugin::toolTip() const
{
    return QString();
}

QString CYCorePlugin::whatsThis() const
{
    return QString();
}

bool CYCorePlugin::isContainer() const
{
    return false;
}

QString CYCorePlugin::domXml() const
{
    return "<widget class=\"CYCore\" name=\"cYCore\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>100</width>\n"
        "   <height>100</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QString CYCorePlugin::includeFile() const
{
    return "cycore.h";
}

