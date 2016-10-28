#include "testprocessor.h"

#include <QtCore/QtPlugin>
#include "testprocessorplugin.h"


testProcessorPlugin::testProcessorPlugin()
{
}
testProcessorPlugin::~testProcessorPlugin()
{

}
bool testProcessorPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    return true;
}

void testProcessorPlugin::extensionsInitialized()
{

}
