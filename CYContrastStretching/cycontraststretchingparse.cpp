#include "cycontraststretchingparse.h"

class CYContrastStretchingParsePrivate {
public:
    
};

CYContrastStretchingParse::CYContrastStretchingParse() :
    CYCore::CYProcessor(0)
{
    AllocFrameBuffer();
}

CYContrastStretchingParse::~CYContrastStretchingParse()
{
    
}
void CYContrastStretchingParse::doProcess(CYFRAME &frame)
{
    
}
QWidget * CYContrastStretchingParse::widget()
{
    return new QWidget;
}