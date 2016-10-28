#include "test2processor.h"

#include <QtCore/QtPlugin>
#include "test2processorplugin.h"

test2ProcessorPlugin::test2ProcessorPlugin()
{
}
test2ProcessorPlugin::~test2ProcessorPlugin()
{

}
bool test2ProcessorPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    return true;
}

void test2ProcessorPlugin::extensionsInitialized()
{

}
