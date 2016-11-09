#pragma once

#include <QWidget>
class ContrastsPlotPrivate;
class ContrastsPlot : public QWidget
{
    Q_OBJECT

public:
    ContrastsPlot(QWidget *parent);
    ~ContrastsPlot();
    void setWidth(int val);
    void setHeight(int val);
    void updateData(double * p, size_t numberpoint);
private:
    friend class ContrastsPlotPrivate;
    ContrastsPlotPrivate * d_ptr;
};
