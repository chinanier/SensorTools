#ifndef CYCONSTRASTSTRETCHINGPARSE_H
#define CYCONSTRASTSTRETCHINGPARSE_H

#include "cycore/cyprocessor.h"
using namespace CYCore;
class CYContrastStretchingParse : public CYProcessor
{
    Q_OBJECT

public:
    CYContrastStretchingParse();
    ~CYContrastStretchingParse();
    virtual void doProcess(CYFRAME &frame);
private:
    
};

#endif // CYCONSTRASTSTRETCHINGPARSE_H
