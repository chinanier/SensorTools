#include "test2camera2.h"

#include <QtCore/QtPlugin>
#include "test2camera2plugin.h"
#include "type2camerafactory2.h"

test2Camera2Plugin::test2Camera2Plugin()
{
}
test2Camera2Plugin::~test2Camera2Plugin()
{

}
bool test2Camera2Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Type2CameraFactory2 *camerafactory = new Type2CameraFactory2;
    addAutoReleasedObject(camerafactory);
    return true;
}

void test2Camera2Plugin::extensionsInitialized()
{

}