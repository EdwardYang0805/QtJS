#include "Qt5InterfaceVSHtmlInterface.h"

Qt5InterfaceVSHtmlInterface::Qt5InterfaceVSHtmlInterface(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pQtWeb = new QtWeb(this);
	ui.verticalLayout->addWidget(m_pQtWeb);
	m_pQtWeb->loadUrl("file:///./html/line.html");
	connect(m_pQtWeb, SIGNAL(msgLoadFinish(bool)), this, SLOT(domsgLoadFinish(bool)));
	connect(m_pQtWeb, SIGNAL(msgSendRequest(QString, QString)), this, SLOT(doResponseRequest(QString, QString)));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(domsgClick()));
}

void Qt5InterfaceVSHtmlInterface::domsgLoadFinish(bool bSuccess)
{
	m_loadHtmlIsOk = bSuccess;
}

void Qt5InterfaceVSHtmlInterface::doResponseRequest(QString url, QString data)
{
	QString text = "this message is from Qt";
	if (m_loadHtmlIsOk)
		m_pQtWeb->responseInterFace(url, 200, text, "Text");
}

void Qt5InterfaceVSHtmlInterface::domsgClick()
{
	if (m_loadHtmlIsOk)
		m_pQtWeb->webInterFace("clear", "none");
}