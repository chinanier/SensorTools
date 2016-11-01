#ifndef CAMYUPROPERTYEDIT_H
#define CAMYUPROPERTYEDIT_H

#include "camyupropertyedit_global.h"
#include <QWidget>
namespace CYCore {
    class CYCamera;
    class CYCameraFactory;
}
namespace CAMYUPROPEDIT{
class CamyuPropertyEditPrivate;
class CAMYUPROPERTYEDIT_EXPORT CamyuPropertyEdit : public QWidget
{
    Q_OBJECT
public:
    CamyuPropertyEdit();
    ~CamyuPropertyEdit();
    void bindCamera(CYCore::CYCamera*pCamera);
    void bindCameraFactory(CYCore::CYCameraFactory * pfactory);
private:
    friend class CAMYUPROPEDIT::CamyuPropertyEditPrivate;
    CAMYUPROPEDIT::CamyuPropertyEditPrivate * d;
};
}
#endif // CAMYUPROPERTYEDIT_H
