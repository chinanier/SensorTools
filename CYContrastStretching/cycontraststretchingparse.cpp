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
    // ͳ�ƻҶ�
    int imagebit = frame.s_bw;
    unsigned char * pimage = (unsigned char *)frame.s_data;
    // ֱ�ӵ��ɻҶ�ͼ������ʾ
    for (int h = 0;h<frame.s_height;h++)
    {
        for (int w = 0;w<frame.s_width;w++)
        {
            
        }
    }
    // ��ʾͼ��
    
}
QWidget * CYContrastStretchingParse::widget()
{
    return d->confwidget;
}