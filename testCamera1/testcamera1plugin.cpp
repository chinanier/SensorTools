#include "testcamera1.h"

#include <QtCore/QtPlugin>
#include "testcamera1plugin.h"
#include "type1camerafactory.h"

testCamera1Plugin::testCamera1Plugin()
{
}
testCamera1Plugin::~testCamera1Plugin()
{

}
bool testCamera1Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Type1CameraFactory *camerafactory = new Type1CameraFactory;
    addAutoReleasedObject(camerafactory);
    return true;
}

void testCamera1Plugin::extensionsInitialized()
{

}