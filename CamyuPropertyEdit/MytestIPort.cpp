#include "MytestIPort.h"

#include <QDebug>
#include "clser.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"visualc\\clsersis.lib")

#define GAIN_MAX_REGISTER   (0xf0f00520)   // 31~20
#define GAIN_MIN_REGISTER   (0xf0f00520)   // 8~19
#define GAIN_IMP_REGISTER   (0xf0f00820)   // 19~8
#define GAIN_VALUE_REGISTER (0xf0f00820) // 31~20

#define  BASE_ADDR_DEBUG			0x1000 //摄像机调试模式基地址


#define SER_SENDOVERTIME			5000
#define SER_RECVOVERTIME			5000



#define HEADER_SIZE                  2
#define HEADER_7E                    0x7E
#define HEADER_E7                    0xE7
#define WRITE_CMD_ACK_LEN            (HEADER_SIZE + 6)
#define READ_CMD_ACK_LEN             WRITE_CMD_ACK_LEN

#define  CAMERA_ID                   1


//读取串口返回数据
#define INPUT_PARAM_ERROR			0x11
#define RECV_TIMEOUT_NO_DATA		0x22
#define RECV_DATA_TOO_LEN			0x33
#define RECV_DATA_ERROR				0x44
#define RECV_DATA_NO_HEADER			0x55
#define RECV_DATA_LEN_ERROR			0x66
#define RECV_DATA_CHECK_ERROR		0x77
#define RECV_DATA_OK				0x88

void testRead(void * m_serialRef)
{
    for (int i = 0;i<512;i++)
    {
        
    }
}

MytestIPort::MytestIPort()
{
    // 获取串口ID //GetSerialPortId
    int i = 0;
    unsigned int nSerPortNum;
    unsigned int buflen = 0;
    if (clGetNumSerialPorts(&nSerPortNum) != CL_ERR_NO_ERR)
        return;
    if (i >= nSerPortNum || i < 0)
    {
        return;
    }
    if (clGetSerialPortIdentifier(i, m_cacheBuff, &buflen) == CL_ERR_INVALID_INDEX)
        i = 0;
    m_cacheBuff = new char[buflen];
    if (clGetSerialPortIdentifier(i, m_cacheBuff, &buflen) != CL_ERR_NO_ERR)
    {
        delete[] m_cacheBuff;
        m_cacheBuff = nullptr;
        return;
    }
    delete[] m_cacheBuff;m_cacheBuff = nullptr;
    m_nPortID = i;
    //OpenDllPort
    if (clSerialInit(m_nPortID, &m_serialRef) != CL_ERR_NO_ERR)
    {
        return;
    }
    if (CL_ERR_NO_ERR != clSetBaudRate(m_serialRef, m_nBaudRate))
    {
        return;
    }
    if (clFlushPort(m_serialRef) != CL_ERR_NO_ERR)
    {
        return;
    }
    // 测试读取数据
    
}


MytestIPort::~MytestIPort()
{
    if (m_serialRef)
    {
        clSerialClose(m_serialRef);
    }
}
bool MytestIPort::SerReadData(char* buffer, unsigned int numBytes, unsigned int serialTimeout)
{
    if (m_serialRef == NULL)
        return false;

    if (CL_ERR_NO_ERR != clSerialRead(m_serialRef, buffer, &numBytes, serialTimeout))
        return false;

    //	AfxMessageBox(m_strAckInfo);
    return true;
}
bool MytestIPort::FlushPort(void)
{
    if (clFlushPort(m_serialRef) != CL_ERR_NO_ERR)
    {
        return false;
    }
    return true;
}
bool MytestIPort::SerWriteData(char* buffer, unsigned int numBytes, unsigned int serialTimeout)
{
    if (m_serialRef == NULL)
        return false;

    if (CL_ERR_NO_ERR != clSerialWrite(m_serialRef, buffer, &numBytes, serialTimeout))
        return false;
    return true;
}
bool MytestIPort::GetNumBytesAvail(unsigned int* numBytes)
{
    if (clGetNumBytesAvail(m_serialRef, numBytes) != CL_ERR_NO_ERR)
    {
        return false;
    }
    return true;
}
int MytestIPort::CYUSB_BulkRead(void* buffer, DWORD nName, DWORD length, int nDev, bool bWriteAck, int* pDataLength)
{
    UCHAR *pBuf = (UCHAR *)buffer;
    UCHAR checksum = 0;
    UCHAR nLen = 0;
    UINT NumberBytesIn = 0;
    UINT nStillLen = 0;
    UINT i = 0;
    UINT nReadFlag = 0;
    char localbuf[256] = { 0 };
    UINT nFirstReadTimes = 0;
    int nReadAgainLen = 0;
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    //第一次如果读不到数据，必须超时退出;
    while (1)
    {
        if ((nFirstReadTimes < 80))
        {
            GetNumBytesAvail(&NumberBytesIn);
            nFirstReadTimes++;
            if (NumberBytesIn >= length)
            {
                break;
            }
        }
        else
        {
            break;
        }
        Sleep(1);
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    //1. 数据超出，所读长度，一次读完;
    if (NumberBytesIn == 0)
    {
        //CString str;
        //str.Format("接收数据超时,nCommad = %x", nName);
        //		MessageBox(NULL,str,"RecvErr",MB_OK);
        printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
        return RECV_TIMEOUT_NO_DATA;							//没有接收到数据;
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    if (NumberBytesIn > 255)
    {
        FlushPort();
        printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
        return RECV_DATA_TOO_LEN;
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    nLen = (UCHAR)NumberBytesIn;
    //2. 读帧头，一直要读到帧头0x7e, e7
    for (int j = 0; j < 2; j++)
    {
        if (SerReadData(localbuf, nLen, 80) == FALSE)
        {
            printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
            return RECV_DATA_ERROR;								//接收数据出错;
        }
        printf("[%s:%d][", __FUNCTION__, __LINE__);
        for (i = 0;i < nLen-1; i++)
        {
            printf("%02X ",(unsigned char)localbuf[i]);
        }
        printf("%02X]\r\n", localbuf[i]);
        // 查找信令头,通过i记录信令头的位置
        for (i = 0;i < nLen; i++)
        {
            if ((UCHAR)localbuf[i] == 0x7e)
            {
                if ((UCHAR)localbuf[i + 1] == 0xe7)
                {
                    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
                    nReadFlag = 1;
                    break;
                }
            }
        }
        if (nReadFlag)
        {
            printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
            break;
        }
    }
    if (!nReadFlag)
    {
        printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
        return RECV_DATA_NO_HEADER;								//没有读到帧头
    }

    {
        //3. 去掉前面的多余数据;
        if (i)
        {
            //4. 一包数据没有读完，继续读;
            nStillLen = i;											//下一个的baseAddress:
            if ((nLen + nStillLen) > 128)
            {
                return RECV_DATA_TOO_LEN;
            }
            else
            {
                nReadAgainLen = localbuf[nStillLen + HEADER_SIZE];       // ？？？ 这个是什么意思
                nReadAgainLen = nReadAgainLen - (nLen - HEADER_SIZE - 2); // 除去:两个帧头和一个设备地址(3),长度位和校验和位(2)
                if (nReadAgainLen < 0)
                {
                    nReadAgainLen = 0;
                }
                if (SerReadData(localbuf + nLen, nReadAgainLen, 80) == FALSE)
                {
                    return RECV_DATA_ERROR;
                }
            }
        }
        else
        {

            nReadAgainLen = localbuf[nStillLen + HEADER_SIZE];
            //sprintf(test,"%d %d\n",nReadAgainLen,nLen);
            nReadAgainLen = nReadAgainLen - (nLen - HEADER_SIZE - 2);
            if (nReadAgainLen < 0)
            {
                nReadAgainLen = 0;
            }

            //OutputDebugString(test);
            if (nReadAgainLen)
            {
                if (SerReadData(localbuf + nLen, nReadAgainLen, 80) == FALSE)
                {
                    return RECV_DATA_ERROR;
                }
            }
        }
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    nLen = localbuf[nStillLen + HEADER_SIZE];	                               // 长度,(命令+数据)
    nName = (localbuf[nStillLen + HEADER_SIZE + 1] << 8) + localbuf[nStillLen + HEADER_SIZE + 2];  // 命令
    checksum = 0;
    printf("[%s:%d]nLen=%d,nName=%d\r\n", __FUNCTION__, __LINE__,nLen,nName);
    // 计算校验和
    for (int j = 0; j < (UINT)(nLen + 1);j++)							//校验和包括长度;长度加命令加数据
    {
        checksum += localbuf[nStillLen + j + HEADER_SIZE]; // 从固定值开始计算校验和
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    if (checksum != (UCHAR)localbuf[nStillLen + nLen + 1 + HEADER_SIZE])
    {
        return  RECV_DATA_CHECK_ERROR;							 //校验和失败;
    }
    else
    {
        printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
        // 校验成功
        int nReadLen = nLen + HEADER_SIZE + 2;
        if (pDataLength)
        {
            printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
            *pDataLength = nReadLen;
        }
        printf("[%s:%d] copy len = %d\r\n", __FUNCTION__, __LINE__, nReadLen);
        memcpy(pBuf, localbuf + nStillLen, nReadLen);                 //20140107 read error
        printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
        if (bWriteAck)		//如果是写应答才有错误信息
        {
            //		OutputDebugString("44444");

            //   memcpy(TestBuffer,localbuf+nStillLen+i,(nLen+3));  
            if (pBuf[7] == 0x44)
            {
                //MessageBox(NULL, "44H：帧内字节超时", "RecvErr", MB_OK);
                return RECV_DATA_ERROR;
            }
            else if (pBuf[7] == 0x22)//可以升级
            {
                return RECV_DATA_OK;
            }
            else if (pBuf[7] == 0x33)
            {
                //MessageBox(NULL, "33H：请切换到调试工作模式(非法数据)", "RecvErr", MB_OK);
                return RECV_DATA_ERROR;
            }
            else if (pBuf[7] == 0x55)
            {
                //MessageBox(NULL, "55H：写FLASH错误", "RecvErr", MB_OK);
                return RECV_DATA_ERROR;
            }
            else if (pBuf[7] == 0x66)
            {
                //MessageBox(NULL, "66H：读FLASH错误", "RecvErr", MB_OK);
                return RECV_DATA_ERROR;
            }
            else if (pBuf[7] == 0x11)
            {
                return  RECV_DATA_OK;
            }
            else
            {
                return  RECV_DATA_OK;
            }
        }
        else
        {
            return  RECV_DATA_OK;
        }
    }
}


//写固定长命令，然后返回确认信息即可，相当于返回的数据确认！！！
BOOL MytestIPort::SendCommand(UINT nName, WORD nParam, int nDev, bool bDebug)
{
    char temp = 0;
    char checksum = 0;
    char SerSendBuf[12] = { 0 };

    //nName |= BASE_ADDR_DEBUG;
    int array_idx = 0;
    SerSendBuf[array_idx++] = HEADER_7E;// 帧头1
    SerSendBuf[array_idx++] = HEADER_E7;// 帧头2
#if (HEADER_SIZE == 3)
    SerSendBuf[array_idx++] = nDev;     // 设备地址,add by kk 20160516,CY-OPC008相机通信协议（用户版）
#endif

    temp = 0x04;
    checksum += temp;
    SerSendBuf[array_idx++] = 4;        // 数据长度

    temp = (nName & 0xFF00) >> 8;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;     // 地址高位

    temp = nName & 0x00FF;                // 地址低位
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = (nParam & 0xFF00) >> 8;          // 数据高位
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = nParam & 0x00FF;                // 数据低位
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    SerSendBuf[array_idx++] = checksum;  // 校验码
    {
        if (SerWriteData(SerSendBuf, array_idx, SER_SENDOVERTIME) == TRUE)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    //	Sleep(60);
    Sleep(1);
}

BOOL MytestIPort::SendCommandNoDelay(UINT nName, WORD nParam, int nDev, int dataLength)
{
    char temp = 0;
    char checksum = 0;
    char SerSendBuf[12] = { 0 };
    //nName;
    int array_idx = 0;
    SerSendBuf[array_idx++] = HEADER_7E;
    SerSendBuf[array_idx++] = HEADER_E7;
    //SerSendBuf[array_idx++] = nDev;
#if (HEADER_SIZE == 3)
    SerSendBuf[array_idx++] = nDev;     // 设备地址,add by kk 20160516,CY-OPC008相机通信协议（用户版）
#endif
    temp = dataLength + 2;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = (nName & 0xFF00) >> 8;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = nName & 0x00FF;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = (nParam & 0xFF00) >> 8;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    temp = nParam & 0x00FF;
    checksum += temp;
    SerSendBuf[array_idx++] = temp;

    SerSendBuf[array_idx++] = checksum;
    if (checksum == 0x7e)
    {
        int i = 0;
    }
    {
        if (SerWriteData(SerSendBuf, dataLength + array_idx - 2, 100) == TRUE)
        {
            //			return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    Sleep(5);
    return TRUE;
}
//发送变长内容，但是接收固定长度内容
BOOL MytestIPort::SendCommandEx(UINT nName, const char* pParam, CHAR nLength, int nDev, bool bWrite)
{
    char temp = 0;
    char checksum = 0;
    char SerSendExBuf[260] = { 0 };
    WORD nWord = 0;
    WORD nCommand = 0;
    WORD nRet = 0;
    UCHAR serialbufferin[255] = { 0 };
    int array_idx = 0;

    SerSendExBuf[array_idx++] = HEADER_7E;    // 帧头1
    SerSendExBuf[array_idx++] = HEADER_E7;    // 帧头2
    //SerSendExBuf[array_idx++] = nDev;     // 设备地址,add by kk 20160516,CY-OPC008相机通信协议（用户版）
#if (HEADER_SIZE == 3)
    SerSendExBuf[array_idx++] = nDev;     // 设备地址,add by kk 20160516,CY-OPC008相机通信协议（用户版）
#endif

    temp = nLength + 2;				/*Data +2 Bytes Ad9970 address，加两个校验和*/
    checksum += temp;
    SerSendExBuf[array_idx++] = temp;			// length+2

    temp = (nName & 0xFF00) >> 8;
    checksum += temp;
    SerSendExBuf[array_idx++] = temp;

    temp = nName & 0x00FF;
    checksum += temp;
    SerSendExBuf[array_idx++] = temp;

    int i = 0;
    for (i = 0; i < nLength; i++)
    {
        temp = pParam[i];
        checksum += temp;
        SerSendExBuf[i + array_idx] = temp;
    }

    SerSendExBuf[i + array_idx++] = checksum;
    {
        if (SerWriteData(SerSendExBuf, nLength + array_idx, SER_SENDOVERTIME) == TRUE)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
//摄像机基本工作参数
#define  BASE_ADDR_WORK				0x0029                //摄像机基本工作参数基地址
#define  READ_PARAM					BASE_ADDR_WORK+0x000A //读取摄像机单个参数	
void MytestIPort::Read(void *pBuffer, int64_t Address, int64_t Length)
{
    printf("To Call Read Interface,param (%p,and = %llX,%d)\n", pBuffer, Address, Length);
#if 0
    int len = Length;
    short nwidth = htons(4096);
    //printf("To Call Read Interface,param (%p,%x,%d)\n",pBuffer,Address, len);
    printf("To Call Read Interface,param (%p,and = %llX,%d)\n", pBuffer, Address, Length);
    //*((unsigned int*)pBuffer) = /*nwidth <<16|nwidth*//*1<<7*/0x00000080;
    unsigned char * p = (unsigned char*)pBuffer;
    //(*(p + 0)) = 0x01;// xml的<Endianess>节点定义的是传输层看到的设备的字节序
    unsigned int Thisaddr = Address;
    switch (Thisaddr)
    {
    case GAIN_IMP_REGISTER://GAIN_VALUE_REGISTER
        (*(p + 0)) = 0x80;
        (*(p + 2)) = 0x01;
        (*(p + 3)) = 0xe6;
        break;
    case GAIN_MAX_REGISTER://GAIN_MIN_REGISTER
        (*(p + 1)) = 0x04;
        (*(p + 2)) = 0x21;
        (*(p + 3)) = 0xe7;
        break;
    case 0x2000:
        sprintf((char*)pBuffer, "This feature provides!!!!!!!!!!");
        break;
    default:
        break;
    }
#endif
    if (!m_serialRef)
    {
        return;
    }
    UINT nName = Address;
    WORD nCommand = nName;
    WORD nWord = 0;
    UCHAR serialbufferin[128] = { 0 };
    int nReadlen = 0;
    unsigned char * pD = (unsigned char*)pBuffer;
    printf("Read:");
    for (int i = 0;i < Length;i++)
    {
        printf("0x%02X ", pD[i]);
    }
    printf("\r\n");
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    // 先发送一个读命令,然后等待参数返回
    SendCommandNoDelay(/*READ_PARAM*/0x1033, nCommand, CAMERA_ID, 2);						/*读取地址,超时重传*/
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    Sleep(2);
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    nWord = CYUSB_BulkRead(serialbufferin, nCommand, READ_CMD_ACK_LEN, CAMERA_ID, false, &nReadlen);	/*读取的内容*/
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    if (nWord == RECV_DATA_OK)
    {
        // 解析回复的长度 长度等于（地址+实际有效数据个数(2)）
        int nBulkLen = serialbufferin[HEADER_SIZE] - 2;
        // 解析回复的地址
        // 解析回复的数据
        int cpylen = nBulkLen > Length ? Length : nBulkLen;
        memcpy(pBuffer, &serialbufferin[HEADER_SIZE + 1 + 2], cpylen);
        //nWord = (serialbufferin[READ_CMD_ACK_LEN - 3] << 8) + serialbufferin[READ_CMD_ACK_LEN - 2];
        //int nValue = nWord;
        //*((int*)pBuffer) = nValue;
        unsigned char * pD = (unsigned char*)pBuffer;
        printf("Read To:");
        for (int i = 0;i < HEADER_SIZE + 1 + 2;i++)
        {
            printf("0x%02X ", pD[i]);
        }
        printf("\r\n");
    }
    printf("[%s:%d]\r\n", __FUNCTION__, __LINE__);
    return;
}
void MytestIPort::Write(const void *pBuffer, int64_t Address, int64_t Length)
{
    printf("To Call Write Interface,param (%p,%llX,%ld)\n", pBuffer, Address, Length);
    //*((int*)(pBuffer)) = 4096;
    UINT nName = Address;
    WORD nParam = *((int*)pBuffer);
    CHAR len = Length;
    unsigned char * pD = (unsigned char*)pBuffer;
    //SendCommand(nName, nParam, CAMERA_ID, true);
    printf("Write:");
    for (int i = 0;i < len;i++)
    {
        printf("0x%02X ", pD[i]);
    }
    printf("\r\n");
    SendCommandEx(nName, (const char *)pBuffer, len, CAMERA_ID, 0);
    char recvBuf[128] = { 0 };
    int nReadlen = 0;
    int	nWord = CYUSB_BulkRead(recvBuf, nName, READ_CMD_ACK_LEN, CAMERA_ID, 0, &nReadlen);	/*读取的内容*/
    if (nWord == RECV_DATA_OK)
    {
        return;
    }
    return;
}
GenApi::EAccessMode MytestIPort::GetAccessMode() const
{
    printf("To Call GetAccessMode Interface\n");
    //return GenApi::IBase::GetAccessMode();
    return GenApi::EAccessMode::RW;
}