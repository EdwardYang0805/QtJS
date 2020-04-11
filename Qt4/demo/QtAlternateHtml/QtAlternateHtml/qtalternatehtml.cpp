#include "qtalternatehtml.h"
using namespace std;

QtAlternateHtml::QtAlternateHtml(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pQtWeb = new QtWeb(this);
	ui.verticalLayout->addWidget(m_pQtWeb);
	m_pQtWeb->loadUrl("./html/line.html");
	connect(m_pQtWeb, SIGNAL(msgLoadFinish(bool)), this, SLOT(domsgLoadFinish(bool)));
	connect(m_pQtWeb, SIGNAL(msgSendRequest(QString, QString, QString)), this, SLOT(doResponseRequest(QString, QString, QString)));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(domsgClick()));
}

QtAlternateHtml::~QtAlternateHtml()
{

}


void QtAlternateHtml::domsgLoadFinish(bool bSuccess)
{
	m_loadHtmlIsOk = bSuccess;	
}

void QtAlternateHtml::doResponseRequest(QString url, QString data, QString type)
{
	QString text = "this message is from Qt";
	if(m_loadHtmlIsOk)
	 	m_pQtWeb->responseInterFace(url, 200, text, "Text");
}

void QtAlternateHtml::domsgClick()
{
	if(m_loadHtmlIsOk)
		m_pQtWeb->webInterFace("clear", "none");
}
