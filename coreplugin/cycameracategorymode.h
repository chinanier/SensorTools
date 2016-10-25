#ifndef CYCAMERACATEGORYMODE_H
#define CYCAMERACATEGORYMODE_H

#include <coreplugin/imode.h>
#include <Utils/id.h>
using namespace Utils;
class CYCameraCategoryMode : public Core::IMode
{
    Q_OBJECT

public:
    CYCameraCategoryMode(Id m_mode,QObject *parent=0);
    ~CYCameraCategoryMode();

private:
    Id m_mode;
    QWidget * m_widget;
};

#endif // CYCAMERACATEGORYMODE_H
