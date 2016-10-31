#ifndef CAMLINKAD0FACTORY_H
#define CAMLINKAD0FACTORY_H

#include <QObject>
#include <CYCore/cycamerafactory.h>
#include <CYCore/cycamera.h>
class CamlinkAD0Factory : public CYCore::CYCameraFactory
{
    Q_OBJECT

public:
    CamlinkAD0Factory();
    ~CamlinkAD0Factory();
    CYCore::CYCamera *createCamera();
    int SerarchCamera();
};

#endif // CAMLINKAD0FACTORY_H
