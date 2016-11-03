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
CYAnalyzer::CYAnalyzer(CYFrameParserFactory * factory,QObject *parent)
    : CYProcessor(factory,parent),
    d(new CYAnalyzerPrivate(this))
{

}

CYAnalyzer::~CYAnalyzer()
{

}
void CYAnalyzer::doProcess(CYFRAME & frame)
{
    doAnalysis(frame);
}