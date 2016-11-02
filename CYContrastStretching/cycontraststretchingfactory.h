#ifndef CYCONTRASTSTRECTCHINGFACTORY_H
#define CYCONTRASTSTRECTCHINGFACTORY_H

#include <cycore/cyframeparserfactory.h>
using namespace CYCore;
class CYContrastStretchingFactory : public CYFrameParserFactory
{
    Q_OBJECT

public:
    CYContrastStretchingFactory();
    ~CYContrastStretchingFactory();
    CYFrameParser *createFrameParser();
private:
    
};

#endif // CYCONTRASTSTRECTCHINGFACTORY_H
