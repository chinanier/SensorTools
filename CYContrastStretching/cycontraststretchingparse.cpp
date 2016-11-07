#include "cycontraststretchingparse.h"
#include "ui_contraststretching.h"
class CYContrastStretchingParsePrivate {
public:
    CYContrastStretchingParsePrivate(){
        confwidget = new QWidget;
        ui.setupUi(confwidget);
    }
public:
    QWidget * confwidget = nullptr;
private:
    Ui::ConsMain ui;
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
    // 直接当成灰度图像来显示
    for (int h = 0;h<frame.s_height;h++)
    {
        for (int w = 0;w<frame.s_width;w++)
        {
            
        }
    }
    // 显示图像
    
}
QWidget * CYContrastStretchingParse::widget()
{
    return d->confwidget;
}