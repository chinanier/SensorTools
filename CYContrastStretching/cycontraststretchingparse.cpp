#include "cycontraststretchingparse.h"
#include "contrasts/Contrasts.h"
class CYContrastStretchingParsePrivate {
public:
    CYContrastStretchingParsePrivate():
        m_point(new double[65536])
    {
        memset(m_point,0,65536);
        confwidget = new Contrasts;
    }
    ~CYContrastStretchingParsePrivate()
    {
        delete m_point;
    }
public:
    Contrasts * confwidget = nullptr;
    double * m_point = nullptr;
};

CYContrastStretchingParse::CYContrastStretchingParse() :
    CYCore::CYProcessor(0),
    d(new CYContrastStretchingParsePrivate)
{
    AllocFrameBuffer();
}

CYContrastStretchingParse::~CYContrastStretchingParse()
{
    
}
void CYContrastStretchingParse::doProcess(CYFRAME &frame)
{
    // 统计灰度
    int imagebit = frame.s_bw;
    unsigned char * pimage = (unsigned char *)frame.s_data;
    uint * grayTable = d->confwidget->grayTable();
    memset(d->m_point, 0, 65536);
    // 直接当成8位灰度图像来显示
    for (int h = 0;h<frame.s_height;h++)
    {
        for (int w = 0;w<frame.s_width;w++)
        {
            //
            uchar ngray = *(pimage + w + h*frame.s_width);
            d->m_point[ngray]++;
            *(pimage + w + h*frame.s_width) = grayTable[ngray];
        }
    }
    // 显示图像
    
    // 更新曲线
    d->confwidget->updateData(d->m_point, 256);
}
QWidget * CYContrastStretchingParse::widget()
{
    return d->confwidget;
}