#include "ContrastsPlot.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_series_data.h>
//#include <qwt_se>

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QPointF>
#include <QVector>


// cure data - 曲线的数据类
class ContrasBuffer : public QwtSeriesData<QPointF>
{
public:
    ContrasBuffer()
    {
        fill(0, 100);
    }
    virtual ~ContrasBuffer()
    {
        
    }
    void fill(const double * points, size_t numPoints)
    {
        d_values.resize(numPoints);
        d_values.fill(100.0);
        if (points)
        {
            for (size_t i=0;i<numPoints;i++)
            {
                d_values[i] = points[i];
            }
        }
    }
    virtual size_t size() const
    {
        return d_values.size();
    }
    virtual QPointF sample(size_t i) const
    {
        const int size = d_values.size();
        int index = i;
        if (index >= size)
            index = size;
        const double x = i;
        const double y = d_values.data()[index];

        return QPointF(x, y);
    }
    virtual QRectF boundingRect() const
    {
        return QRectF(0,0,10000,10000);
    }
private:
    QVector<double> d_values;
};



class ContrastsPlotPrivate {
public:
    ContrastsPlotPrivate():
        m_plot(new QwtPlot),
        m_canvas(new QwtPlotCanvas),
        m_cure(new QwtPlotCurve),
        m_grid(new QwtPlotGrid)
    {
        //m_plot->setAutoReplot(true);
    }
    ~ContrastsPlotPrivate()
    {
        
    }
protected:
    void init()
    {
        // 设置父类
        QStackedLayout * layout = qobject_cast<QStackedLayout *>( q_ptr->layout());
        if (layout)
        {
            layout->addWidget(m_plot);
            layout->setCurrentWidget(m_plot);
        }
        // 设置绘图画布
        m_plot->setCanvas(m_canvas);
        // 添加表格标线
        m_grid->attach(m_plot);
        m_grid->enableXMin(true);
        m_grid->enableYMin(true);
        m_grid->setPen(Qt::gray);
        // 添加曲线
        m_cure->attach(m_plot);
        m_cure->setPen(Qt::black);
        m_cure->setBrush(Qt::SolidPattern);
        m_cure->setData(new ContrasBuffer);

        // 配置坐标轴,默认64根线  256/4=64 64/4=16,每格四个像素
        m_plot->setAxisScaleDiv(QwtPlot::xBottom, axisXDiv());
        m_plot->setAxisScaleDiv(QwtPlot::yLeft, axisYDiv());
    }
    QwtScaleDiv axisXDiv()
    {
        QList<double> minorticks;
        QList<double> mediumticks;
        QList<double> majorticks;
        minorticks.clear();
        mediumticks.clear();
        majorticks.clear();
        for (int i = 0;i <= m_majortickX;i += 1)
        {
            majorticks += i*(m_minortickX + 1)*m_stepX;
            double minorval = majorticks[i];
            for (int j = 0;j < m_minortickX&&i < m_majortickX;j++)
            {
                minorval += m_stepX;
                minorticks += minorval;
            }
        }
        return QwtScaleDiv(0, majorticks.last(), minorticks, mediumticks, majorticks);
    }
    QwtScaleDiv axisYDiv()
    {
        QList<double> minorticks;
        QList<double> mediumticks;
        QList<double> majorticks;
        minorticks.clear();
        mediumticks.clear();
        majorticks.clear();
        for (int i = 0;i <= m_majortickY;i += 1)
        {
            majorticks += i*(m_minortickY + 1)*m_stepY;
            double minorval = majorticks[i];
            for (int j = 0;j < m_minortickY&&i < m_majortickY;j++)
            {
                minorval += m_stepY;
                minorticks += minorval;
            }
        }
        return QwtScaleDiv(0, majorticks.last(), minorticks, mediumticks, majorticks);
    }
private:
    friend class ContrastsPlot;
    ContrastsPlot * q_ptr = nullptr;
    QwtPlot * m_plot = nullptr;
    QwtPlotCanvas * m_canvas = nullptr;
    QwtPlotCurve  * m_cure = nullptr;
    QwtPlotGrid   * m_grid = nullptr;
    int m_majortickX = 16;
    int m_minortickX = 3;
    int m_stepX = 4;

    int m_majortickY = 8;
    int m_minortickY = 1;
    int m_stepY = 2048;
};
ContrastsPlot::ContrastsPlot(QWidget *parent)
    : QWidget(parent),
    d_ptr(new ContrastsPlotPrivate)
{
    QVBoxLayout * vlayout=0;
    QStackedLayout * layout = new QStackedLayout(this);
    if (parent)
    {
        if (!parent->layout())
        {
            vlayout = new QVBoxLayout(parent);
        }
        vlayout->addWidget(this);
    }
    this->setLayout(vlayout);
    d_ptr->q_ptr = this;
    // 分配plot
    d_ptr->init();
}

ContrastsPlot::~ContrastsPlot()
{
    if (d_ptr)
    {
        delete d_ptr;
        d_ptr = 0;
    }
}
void ContrastsPlot::setWidth(int val)
{
    if (d_ptr)
    {
        d_ptr->m_stepX = val;
        d_ptr->m_plot->setAxisScaleDiv(QwtPlot::xBottom, d_ptr->axisXDiv());
        d_ptr->m_plot->replot();
    }
}
void ContrastsPlot::setHeight(int val)
{
    if (d_ptr)
    {
        d_ptr->m_stepY = val;
        d_ptr->m_plot->setAxisScaleDiv(QwtPlot::yLeft, d_ptr->axisYDiv());
        d_ptr->m_plot->replot();
    }
}
void ContrastsPlot::updateData(double * p, size_t numberpoint)
{
    if (d_ptr)
    {
        ContrasBuffer *buffer = static_cast<ContrasBuffer *>(d_ptr->m_cure->data());
       if (buffer)
       {
           buffer->fill(p, numberpoint);
           d_ptr->m_plot->replot();
       }
    }
}
