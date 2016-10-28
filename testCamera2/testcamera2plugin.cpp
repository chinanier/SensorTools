#include "testcamera2.h"

#include <QtCore/QtPlugin>
#include "testcamera2plugin.h"
#include "type1camerafactory2.h"

testCamera2Plugin::testCamera2Plugin()
{
}
testCamera2Plugin::~testCamera2Plugin()
{

}
bool testCamera2Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Type1CameraFactory2 *camerafactory = new Type1CameraFactory2;
    addAutoReleasedObject(camerafactory);
    return true;
}

void testCamera2Plugin::extensionsInitialized()
{

}
