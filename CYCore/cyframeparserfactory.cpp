#include "cyframeparser.h"
#include "cyframeparserfactory.h"

using namespace CYCore;
CYFrameParserFactory::CYFrameParserFactory(QObject *parent)
    : QObject(parent)
{

}

CYFrameParserFactory::~CYFrameParserFactory()
{

}
CYFrameParser * CYFrameParserFactory::createFrameParser_helper() 
{ 
    CYFrameParser * par = createFrameParser();
    if (par && !par->factory())
    {
        par->setFactory(this);
    }
    return par;
}
