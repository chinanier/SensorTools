#include "camlinkad0.h"

#include <QtCore/QtPlugin>
#include "camlinkad0plugin.h"
#include "camlinkad0factory.h"

CamlinkAD0Plugin::CamlinkAD0Plugin()
{
}
CamlinkAD0Plugin::~CamlinkAD0Plugin()
{

}
bool CamlinkAD0Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    CamlinkAD0Factory *camerafactory = new CamlinkAD0Factory;
    addAutoReleasedObject(camerafactory);
    return true;
}

void CamlinkAD0Plugin::extensionsInitialized()
{

}
