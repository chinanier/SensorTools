#ifndef _SLIDER_BOX_H_
#define _SLIDER_BOX_H_ 1

#include <qwidget.h>

class QLabel;
class QwtSlider;

class SliderBox: public QWidget
{
    Q_OBJECT
public:
    SliderBox(QWidget *parent = NULL , int sliderType =1);
public:
    QwtSlider *getSlider();
private Q_SLOTS:
    void setNum( double v );

private:
    QwtSlider *createSlider( int sliderType ) const;

    QwtSlider *d_slider;
    QLabel *d_label;
};

#endif
