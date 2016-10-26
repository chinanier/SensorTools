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
    // ����������ʲô������,��Ϊû���Լ����ڴ�
    return false;
}
bool CYAnalyzer::pushFullFrame(CYFRAME srcframe, CYFRAME & newframe)
{
    // �������̻߳���,�����������߳�
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