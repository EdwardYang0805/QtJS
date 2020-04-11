#include "qtweb.h"
#include <QWebPage>
#include <QWebFrame>
#include <QDate>
#include <qtimer.h>

QtWeb::QtWeb(QWidget *parent)
	: QWebView(parent)
{
	QTimer *checkTimer = new QTimer(this);
	connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkTimeout()));
	checkTimer->start(1000);
	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(dealLoadFinished(bool)));
	connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(registerJSObj()));
	this->setStyleSheet(QString("QScrollBar::vertical{	background-color:rgb(54,80,79);margin:22px 0 22px 0;border:1px solid rgb(20,144,136);}\
								 QScrollBar::handle:vertical{	background-color:rgb(1,64,71);"));
}

QtWeb::~QtWeb()
{

}

//************************************
// Method:    registerJSObj
// FullName:  QtWeb::registerJSObj
// Access:    public 
// Returns:   void
// Func:      �ۺ��� ע��Qt�Ķ����Ա�HTML����ʹ��
// Qualifier: btz
//************************************
void QtWeb::registerJSObj()
{
	page()->mainFrame()->addToJavaScriptWindowObject(QString("QtWebObj"), this);
}

//************************************
// Method:    loadUrl
// FullName:  QtWeb::loadUrl
// Access:    public 
// Returns:   void
// Func:      ���ϲ��ṩ�Լ����HTML�ķ���
// Qualifier: btz
// Parameter: QString urlStr  html����·��
//************************************
void QtWeb::loadUrl(QString urlStr)
{
	this->load(QUrl(urlStr));
}

//************************************
// Method:    checkTimeout
// FullName:  QtWeb::checkTimeout
// Access:    public 
// Returns:   void
// Func:      ��ʱ���ۺ�������ʱ��������Ƿ���ʱ����ʱ�������ش���
// Qualifier: btz
//************************************
void QtWeb::checkTimeout()
{
	if (m_requestList.empty())
		return;
	foreach(Json::Value oneItem, m_requestList)
	{
		if ((oneItem["timestamp"].asInt() + oneItem["timeout"].asInt()/1000) < QDateTime::currentDateTime().toTime_t())
		{
			responseInterFace(QString::fromStdString(oneItem["url"].asString()), 504, "error", "Text");
			m_requestList.removeOne(oneItem);
		}
	}
}

//************************************
// Method:    dealLoadFinished
// FullName:  QtWeb::dealLoadFinished
// Access:    public 
// Returns:   void
// Func:      ����html�Ĳۺ����������źţ��ϲ�ɸ��ݲ���bSuccess�жϼ���HTML�Ƿ�ɹ�
// Qualifier: btz
// Parameter: bool bSuccess �������html�Ƿ�ɹ�
//************************************
void QtWeb::dealLoadFinished(bool bSuccess)
{
	emit msgLoadFinish(bSuccess);
}

//************************************
// Method:    dealRequest
// FullName:  QtWeb::dealRequest
// Access:    public 
// Returns:   void
// Func:      ����JS�˵����󣬷����źţ����ϲ㴦�����Ƕ�Qt���ṩ�Ľӿڡ�
// Qualifier: btz
// Parameter: QString jsonStr JS�˴����������ʽ��json�ַ���
//************************************
void QtWeb::dealRequest(QString jsonStr)
{
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(jsonStr.toStdString().c_str(), root))
	{
		root["timestamp"] = QDateTime::currentDateTime().toTime_t();
		m_requestList.append(root);
		string url = root["url"].asString();
		string type = root["type"].asString();
		string data = root["data"].asString();
		emit msgSendRequest(QString::fromStdString(url), QString::fromStdString(data), QString::fromStdString(type));
	}
	else
		emit msgRecieveJsonError(400);
}

//************************************
// Method:    responseInterFace
// FullName:  QtWeb::responseInterFace
// Access:    public 
// Returns:   int  0������Ӧ����ɹ� -1����ʧ��
// Func:      ��ӦJS���󣬽��ϲ��ȡ�����ݴ���HTML
// Qualifier: btz
// Parameter: QString url				ָ��
// Parameter: int state					״̬��
// Parameter: QString data				��������
// Parameter: QString dataType			��������
//************************************
int QtWeb::responseInterFace(QString url, int state, QString data, QString dataType /* = "json" */)
{
	int countNum = 0;
	foreach(Json::Value oneItem, m_requestList)
	{
		if (QString::fromStdString(oneItem["url"].asString()) == url)
		{
			Json::Value item;
			item["url"] = url.toStdString();
			item["state"] = state;
			item["datatype"] = dataType.toStdString();
			item["data"] = data.toStdString();
			string jsonStr = item.toStyledString();
			page()->mainFrame()->evaluateJavaScript(QString("responseInterFace(%1)").arg(QString::fromLocal8Bit(jsonStr.c_str())));
			m_requestList.removeOne(oneItem);
			break;
		}
		else
			countNum++;
	}
	if (countNum == m_requestList.length())
		return -1;
	return 0;
}

//************************************
// Method:    webInterFace
// FullName:  QtWeb::webInterFace
// Access:    public 
// Returns:   void
// Func:      Qt�����������źŻ����ݵĽӿ�
// Qualifier: btz
// Parameter: QString url			ָ��
// Parameter: QString dataType		��������
// Parameter: QString data			���͵�����
//************************************
void QtWeb::webInterFace(QString url, QString dataType /* = "json" */, QString data /* = "" */)
{
	Json::Value item;
	item["url"] = url.toStdString();
	item["datatype"] = dataType.toStdString();
	item["data"] = data.toStdString();
	string jsonStr = item.toStyledString();
	page()->mainFrame()->evaluateJavaScript(QString("webInterFace(%1)").arg(QString::fromLocal8Bit(jsonStr.c_str())));
}