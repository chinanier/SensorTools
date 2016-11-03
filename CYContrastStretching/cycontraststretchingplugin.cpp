#include "cycontraststretching.h"

#include <QtCore/QtPlugin>
#include "cycontraststretchingplugin.h"
#include "cycontraststretchingfactory.h"

CYContrastStretchingPlugin::CYContrastStretchingPlugin()
{
}
CYContrastStretchingPlugin::~CYContrastStretchingPlugin()
{

}
bool CYContrastStretchingPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    addAutoReleasedObject(new CYContrastStretchingFactory);
    return true;
}

void CYContrastStretchingPlugin::extensionsInitialized()
{

}
