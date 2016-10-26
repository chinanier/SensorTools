#include "cyanalyzer.h"
#include "cyanalyzer_p.h"
using namespace CYCore;
using namespace Internal;

CYAnalyzerPrivate::CYAnalyzerPrivate(QObject *parent)
    : QObject(parent)
{

}

CYAnalyzerPrivate::~CYAnalyzerPrivate()
{

}
CYAnalyzer::CYAnalyzer(QObject *parent)
    : CYFrameParser(parent)
{

}

CYAnalyzer::~CYAnalyzer()
{

}
void CYAnalyzer::do_exec()
{
    //doAnalysis();
}


bool CYAnalyzer::AllocFrameBuffer()
{
    return true;
}
bool CYAnalyzer::pushEmptyFrame()
{
    // 分析器可以什么都不做,因为没有自己的内存
    return false;
}
bool CYAnalyzer::pushFullFrame(CYFRAME srcframe, CYFRAME & newframe)
{
    // 不是主线程环境,不会阻塞主线程
    newframe = srcframe;
    doAnalysis(newframe);
    return true;
}
bool CYAnalyzer::popupEmptyFrame()
{
    return true;
}
bool CYAnalyzer::popupFullFrame()
{
    return true;
}