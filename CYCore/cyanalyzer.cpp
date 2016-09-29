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
    doAnalysis();
}


void CYAnalyzer::AllocFrameBuffer()
{
    
}
void CYAnalyzer::pushEmptyFrame()
{
    // 分析器可以什么都不做,因为没有自己的内存
    return;
}
void CYAnalyzer::pushFullFrame()
{
    return;
}
void CYAnalyzer::popupEmptyFrame()
{
    return;
}
void CYAnalyzer::popupFullFrame()
{
    return;
}