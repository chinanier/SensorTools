#include <QtCore/QtPlugin>
#include "test2camera1plugin.h"
#include "type2camerafactory.h"


test2Camera1Plugin::test2Camera1Plugin()
{
}
test2Camera1Plugin::~test2Camera1Plugin()
{
    
}
bool test2Camera1Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Type2CameraFactory *camerafactory = new Type2CameraFactory;
    addAutoReleasedObject(camerafactory);
    return true;
}

void test2Camera1Plugin::extensionsInitialized()
{

}