#ifndef CYCONSTRASTSTRETCHINGPARSE_H
#define CYCONSTRASTSTRETCHINGPARSE_H

#include "cycore/cyprocessor.h"
using namespace CYCore;
class CYContrastStretchingParsePrivate;
class CYContrastStretchingParse : public CYProcessor
{
    Q_OBJECT

public:
    CYContrastStretchingParse();
    ~CYContrastStretchingParse();
    virtual void doProcess(CYFRAME &frame);
    virtual QWidget * widget();
private:
    CYContrastStretchingParsePrivate * d;
    friend class CYContrastStretchingParsePrivate;
};

#endif // CYCONSTRASTSTRETCHINGPARSE_H
