#pragma once

#include <QtWidgets/QWidget>
#include "ui_Contrasts.h"

class ContrastsPrivate;
class Contrasts : public QWidget
{
    Q_OBJECT

public:
    Contrasts(QWidget *parent = Q_NULLPTR);
    void updateData(double * p,size_t numberpoint);
    uint * grayTable();
private:
    friend class ContrastsPrivate;
    ContrastsPrivate * d_ptr;
    Ui::ContrastsClass ui;
};
