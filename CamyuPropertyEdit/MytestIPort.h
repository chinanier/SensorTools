#pragma once
#include "GenApi\IPort.h"
#include "clser.h"
class MytestIPort :
    public GenApi::IPort
{
public:
    MytestIPort();
    virtual ~MytestIPort();
public:
    //! Reads a chunk of bytes from the port
    virtual void Read(void *pBuffer, int64_t Address, int64_t Length);

    //! Writes a chunk of bytes to the port
    virtual void Write(const void *pBuffer, int64_t Address, int64_t Length);
    virtual GenApi::EAccessMode GetAccessMode() const ;
private:
    bool							SerWriteData(char* buffer, unsigned int numBytes, unsigned int serialTimeout);
    bool							SerReadData(char* buffer, unsigned int numBytes, unsigned int serialTimeout);
    bool							GetNumBytesAvail(unsigned int *numBytes);
    bool							FlushPort(void);
    BOOL SendCommandEx(UINT nName, const char* pParam, CHAR nLength, int nDev, bool bWrite);
    BOOL SendCommandNoDelay(UINT nName, WORD nParam, int nDev, int dataLength);
    BOOL SendCommand(UINT nName, WORD nParam, int nDev, bool bDebug);
    int CYUSB_BulkRead(void* buffer, DWORD nName, DWORD length, int nDev, bool bWriteAck, int* pDataLength);
private:
    int m_nPortID = 0;
    int m_nBaudRate = CL_BAUDRATE_115200;
    char * m_cacheBuff = nullptr;
    void* m_serialRef = nullptr;
};
