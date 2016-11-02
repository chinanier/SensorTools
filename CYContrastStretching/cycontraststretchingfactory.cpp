#include "cycontraststretchingfactory.h"
#include "cycontraststretchingparse.h"
CYContrastStretchingFactory::CYContrastStretchingFactory()
{
    setId("CYContrastStretching");
    setDisplayName("ContrastStretching");
    setType(CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR);
}

CYContrastStretchingFactory::~CYContrastStretchingFactory()
{
    
}
CYFrameParser *CYContrastStretchingFactory::createFrameParser()
{
    return new CYContrastStretchingParse;
}