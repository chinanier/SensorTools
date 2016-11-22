#include "camlinkad0.h"
#include <QMessageBox>

#include <fgrab_struct.h>
#include <fgrab_prototyp.h>
#include <fgrab_define.h>
#include <SisoDisplay.h>
#include <siso_genicam.h>
struct fg_apc_data {
    Fg_Struct*		fg;
    unsigned int	port;
    dma_mem*		mem;
    int				displayid;
    void *        pContext;
    //CyLinker * _linker;
};
class CamlinkAD0Private {
public:
    CamlinkAD0Private()
    {
        m_pMem = new uchar[m_width*m_height*nbBuffers];
        for (int i=0;i<nbBuffers;i++)
        {
            CYFRAME frame = { 0 };
            frame.s_length = m_width*m_height;
            frame.s_data = m_pMem + m_width*m_height*i;
            m_listEmpty << frame;
        }
    }
    ~CamlinkAD0Private()
    {
        if (m_pMem)
        {
            delete[] m_pMem;
            m_pMem = 0;
        }
    }
    //char  m_dllName[512] = "SingleAreaGray16";
    char  m_dllName[512] = "DualAreaGray16";
    int m_camPort = PORT_A;
    int m_boardNr = /*1*/0;
    int64_t m_width = 1024;
    int64_t m_height = 1024;
    dma_mem * m_memHandle = NULL;
    Fg_Struct * m_fg = NULL;
    int samplePerPixel = 1;
    int bytePerSample = 1;
    int nbBuffers = 10;
    struct FgApcControl		m_sFgApcCtrl;
    struct fg_apc_data		m_sFgApcData;
    QList<CYFRAME>          m_listEmpty;
private:
    uchar * m_pMem;
};
void ErrorMessage(Fg_Struct* fg,char * strname)
{
    char nErrorBuf[2048];
    int error = Fg_getLastErrorNumber(fg);
    //	const char* err_str = Fg_getLastErrorDescription( fg );
    const char* err_str = Fg_getLastErrorDescription(fg);
    char* strErr;
    strErr = nErrorBuf;
    sprintf(nErrorBuf, "call [%s] Error: %d : %s!", strname, error, err_str);
    QMessageBox::warning(0, "CamlinkAD0 Init", nErrorBuf);
    //MessageBox(NULL, strErr, "CamlinkBoard", MB_OK);
}
CamlinkAD0::CamlinkAD0()
{
    d = new CamlinkAD0Private;
    connect(this, &CYCamera::sigCompleteFrame, this, &CamlinkAD0::slotCompleteFrame);
}
CamlinkAD0::~CamlinkAD0()
{
    CamlinkAD0Private * p = (CamlinkAD0Private*)d;
    delete p;
    d = 0;
}
bool CamlinkAD0::SerachCamera()
{
    return false;
}
bool CamlinkAD0::connectCamera(int chl)
{
    //bool ret = isConnect(chl) ? false : m_isconnect = true;
    if (!isConnect(chl))
    {
        d->m_fg = Fg_Init(d->m_dllName, d->m_boardNr);    // 连接指定的采集卡
        if (d->m_fg == NULL)
        {
            ErrorMessage(d->m_fg, "Fg_Init");
            return false;    // 连接失败
        }
        int retValue = 0;
        int ret = Fg_getParameterWithType(d->m_fg, FG_PORT, &retValue, 0);
        if (ret != FG_OK)
        {
            ErrorMessage(d->m_fg, "Fg_getParameterWithType");
        }
        ret = Fg_getParameterWithType(d->m_fg, FG_CAMSTATUS, &retValue, 0);
        if (ret != FG_OK)
        {
            ErrorMessage(d->m_fg, "Fg_getParameterWithType");
        }
        ret = Fg_getParameterWithType(d->m_fg, FG_CAMSTATUS_EXTENDED, &retValue, 0);
        if (ret != FG_OK)
        {
            ErrorMessage(d->m_fg, "Fg_getParameterWithType");
        }
//         ret = Fg_getParameterWithType(d->m_fg, FG_DMA_PORT, &retValue, 0);
//         if (ret != FG_OK)
//         {
//             ErrorMessage(d->m_fg, "Fg_getParameterWithType");
//         }
        //Fg_getParameter(d->m_fg, FG_CAMSTATUS, ,0);
        m_isconnect = true;
        return true;
    }
    else
    {
        return true;
    }
}
bool CamlinkAD0::disconnectCamera(int chl)
{
    //return isConnect(chl) ? m_isconnect = false : false;
    stopCapture(chl);
    if (isConnect(chl))
    {
        Fg_FreeGrabber(d->m_fg);
        m_isconnect = false;
    }
    return true;
}
bool CamlinkAD0::isConnect(int chl)
{
    return m_isconnect;
}

bool CamlinkAD0::startCapture(int chl)
{
    //return isCapture(chl) ? false : m_iscapture = true;
    if (!isConnect(chl))
    {
        QMessageBox::information(0, "CamLinkAD0 Capture", "Please Connect Camera!");
        return false;
    }
    /*Image width of the acquisition window.*/
    if (Fg_setParameter(d->m_fg, FG_WIDTH, &d->m_width, d->m_camPort) < 0) {
        ErrorMessage(d->m_fg,"Fg_setParameter:width");

        return false;
    }
    /*Image height of the acquisition window.*/
    if (Fg_setParameter(d->m_fg, FG_HEIGHT, &d->m_height, d->m_camPort) < 0) {
        ErrorMessage(d->m_fg, "Fg_setParameter:height");
        return false;
    }
    // 配置采集类型
    uint32_t appletPixelFormat = FG_CL_DUALTAP_8_BIT;
    int ret = Fg_setParameterWithType(d->m_fg, FG_CAMERA_LINK_CAMTYPE, &appletPixelFormat, 0, FG_PARAM_TYPE_UINT32_T);
    if (ret<0)
    {
        ErrorMessage(d->m_fg, "Fg_setParameterWithType:CamTap");
        return false;
    }
    // 配置对齐属性
    int bitAlignment = FG_LEFT_ALIGNED;
    if (Fg_setParameter(d->m_fg, FG_BITALIGNMENT, &bitAlignment, d->m_camPort) < 0) {
        ErrorMessage(d->m_fg,"Fg_setParameter:Aligned");
        return false;
    }
    size_t totalBufferSize = d->m_width * d->m_height * d->samplePerPixel * d->bytePerSample * d->nbBuffers;
    d->m_memHandle = Fg_AllocMemEx(d->m_fg, totalBufferSize, d->nbBuffers);
    if (d->m_memHandle == NULL) {
        ErrorMessage(d->m_fg,"Fg_AllocMemEx");
        return false;
    }
    /** 注册一个回掉函数 */
    d->m_sFgApcData.port = d->m_camPort;
    d->m_sFgApcData.fg = d->m_fg;
    d->m_sFgApcData.mem = d->m_memHandle;
    d->m_sFgApcData.displayid = 0;
    d->m_sFgApcData.pContext = this;

    d->m_sFgApcCtrl.version = 0;
    d->m_sFgApcCtrl.data = &d->m_sFgApcData;
    d->m_sFgApcCtrl.func = [](frameindex_t picNr, struct fg_apc_data *data)->int {
        CamlinkAD0 * pThis = (CamlinkAD0*)data->pContext;
        uchar * imagePtemp = (uchar*)Fg_getImagePtrEx(data->fg, picNr, data->port, data->mem);
        if (!imagePtemp)
        {
            return 0;
        }
        // 获取图像的长度
        int Value(picNr);
        Fg_getParameterWithType(data->fg, FG_TRANSFER_LEN, &Value, 0, FG_PARAM_TYPE_UINT32_T);
        if (pThis->d->m_listEmpty.size() > 0)
        {
            CYFRAME frame = pThis->d->m_listEmpty.takeFirst();
            frame.s_color = PIX_COLOR_GRAY;
            frame.s_width = pThis->d->m_width;
            frame.s_height = pThis->d->m_height;
            frame.s_seqno = picNr;
            frame.s_piclen = Value;
            uint  cpsize = frame.s_length > Value ? frame.s_length : Value;
            memcpy(frame.s_data, imagePtemp, cpsize);
            emit pThis->sigHaveNewFrame(frame);
            return 0;
        }
    };
    d->m_sFgApcCtrl.flags = FG_APC_DEFAULTS;
    d->m_sFgApcCtrl.timeout = 2147483640;			//5;
    if (Fg_registerApcHandler(d->m_fg, d->m_camPort, &d->m_sFgApcCtrl, FG_APC_CONTROL_BASIC) != FG_OK)
    {
        ErrorMessage(d->m_fg,"Fg_registerApcHandler");
        Fg_FreeMemEx(d->m_fg, d->m_memHandle);d->m_memHandle = 0;
        return false;
    }
    /** 请求采集图像 */
    if ((Fg_AcquireEx(d->m_fg, 0, GRAB_INFINITE, ACQ_STANDARD, d->m_memHandle)) < 0) {
        ErrorMessage(d->m_fg,"Fg_AcquireEx");
        Fg_FreeMemEx(d->m_fg, d->m_memHandle);d->m_memHandle = 0;
        return false;
    }
    m_iscapture = true;
}
bool CamlinkAD0::stopCapture(int chl)
{
    //return isCapture(chl) ? m_iscapture = false : false;
    if (isCapture(chl))
    {
        Fg_registerApcHandler(d->m_fg, 0, NULL, FG_APC_CONTROL_BASIC);
        Fg_stopAcquireEx(d->m_fg, 0, d->m_memHandle, 0);
        Fg_FreeMemEx(d->m_fg, d->m_memHandle);
        m_iscapture = false;
        return true;
    }
    return true;
}
bool CamlinkAD0::isCapture(int chl)
{
    return m_iscapture;
}

bool CamlinkAD0::sendCommand()
{
    return false;
}
bool CamlinkAD0::readCommand()
{
    return false;
}

bool CamlinkAD0::currentFrame()
{
    return false;
}
void CamlinkAD0::slotCompleteFrame(CYFRAME frame)
{
    d->m_listEmpty << frame;
}
