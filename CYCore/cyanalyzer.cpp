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
    // ����������ʲô������,��Ϊû���Լ����ڴ�
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