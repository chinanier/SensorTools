#include "Contrasts.h"
#include "ContrastsPlot.h"
#include "sliderbox.h"

#include "qwt_slider.h"

#include <QDebug>
#include <QtMath>
#include <QSpinBox>
#include <QThread>
#include <QTimer>

class ContrastsPrivate
{
public:
    ContrastsPrivate():m_grayTable(new uint[65536])
    {
        scalGrayTable();
    }
    ~ContrastsPrivate()
    {
        delete m_grayTable;
    }
protected:
    void slotValueChanged(int i)
    {
        //QSpinBox * spinBox = qobject_cast<QSpinBox*> (sender());
    }
    // 计算灰度拉升表
    void scalGrayTable() {
        double dk = 256 / m_grayWidget;
        double kg = dk * m_grayStart;
        for (int i=0;i<256;i++)
        {
            double nG = i*dk - kg + .5;
            nG = nG > 255 ? 255 : (nG<0?0:nG);
            m_grayTable[i] = nG;
        }
    }
private:
    friend class Contrasts;
    Contrasts * q_ptr;
    ContrastsPlot * m_plot;
    uint * m_grayTable;
    double m_grayStart = 0.0;
    double m_grayWidget = 255.0;
};
Contrasts::Contrasts(QWidget *parent)
    : QWidget(parent),
    d_ptr(new ContrastsPrivate)
{
    d_ptr->q_ptr = this;
    ui.setupUi(this);
    d_ptr->m_plot = new ContrastsPlot(ui.widget_plot);
    void (QSpinBox::*func)(int) = &QSpinBox::valueChanged;
    //
    ui.sliderbox_ck->getSlider()->setScale(1.0, 256);
    ui.sliderbox_ck->getSlider()->setTotalSteps(256);
    ui.sliderbox_ck->getSlider()->setSingleSteps(1);
    ui.sliderbox_ck->getSlider()->setValue(256);
    ui.sliderbox_cw->getSlider()->setScale(0.0, 0.0);
    QObject::connect(ui.sliderbox_ck->getSlider(), &QwtAbstractSlider::valueChanged, this, [this](double val) {
        ui.sliderbox_cw->getSlider()->setScale(0.0, 256.0 - val);
        double valcw = ui.sliderbox_cw->getSlider()->value();
        d_ptr->m_grayWidget = val;
        d_ptr->m_grayStart = valcw;
        d_ptr->scalGrayTable();
    });
    QObject::connect(ui.sliderbox_cw->getSlider(), &QwtAbstractSlider::valueChanged, this, [this](double val) {
        double valck = ui.sliderbox_ck->getSlider()->value();
        d_ptr->m_grayStart = val;
        d_ptr->m_grayWidget = valck;
        d_ptr->scalGrayTable();
    });
    // 
    QObject::connect(ui.spinBox, func, this, [this](int i) {
        static int val = 0;
        static int last = 0;
        static int current = -1;
        if (current==i)
        {
            return;
        }
        int dit = i - last;
        last = i;
        if (dit>0)
        {
            val++;
        }
        else
        {
            val--;
        }
        //if (i != qPow(2, val))
        {
            last = current = qPow(2, val);
            ui.spinBox->setValue(qPow(2, val));
            if (last != 0)
            {
                d_ptr->m_plot->setWidth(last);
            }
        }
        qDebug() << "There" << i;
        
    });
    QObject::connect(ui.spinBox_2, func, this, [this](int i) {
        static int val = 0;
        static int last = 0;
        static int current = -1;
        if (current == i)
        {
            return;
        }
        int dit = i - last;
        last = i;
        if (dit > 0)
        {
            val++;
        }
        else
        {
            val--;
        }
        //if (i != qPow(2, val))
        {
            last = current = qPow(2, val);
            ui.spinBox_2->setValue(qPow(2, val));
            if (last != 0)
            {
                d_ptr->m_plot->setHeight(last);
            }
        }
        qDebug() << "There" << i;

    });
}
uint * Contrasts::grayTable()
{
    return d_ptr->m_grayTable;
}
void Contrasts::updateData(double * p, size_t numberpoint)
{
    if (thread() == QThread::currentThread())
    {
        int i = 0;
        i = i;
    }
    if (QThread::currentThread()==qApp->thread())
    {
        if (d_ptr)
        {
            d_ptr->m_plot->updateData(p, numberpoint);
        }
    }
    else
    {
        QTimer::singleShot(0, this, [this,p,numberpoint]() {
            if (d_ptr)
            {
                d_ptr->m_plot->updateData(p, numberpoint);
            }
        });
    }
}
//#include "Contrasts.moc"
