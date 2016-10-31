#ifndef CAMYUPROPERTYEDIT_H
#define CAMYUPROPERTYEDIT_H

#include "camyupropertyedit_global.h"
#include <QWidget>
namespace CAMYUPROPEDIT{
class CamyuPropertyEditPrivate;
class CAMYUPROPERTYEDIT_EXPORT CamyuPropertyEdit : public QWidget
{
    Q_OBJECT
public:
    CamyuPropertyEdit();
    ~CamyuPropertyEdit();

private:
    friend class CAMYUPROPEDIT::CamyuPropertyEditPrivate;
    CAMYUPROPEDIT::CamyuPropertyEditPrivate * d;
};
}
#endif // CAMYUPROPERTYEDIT_H
