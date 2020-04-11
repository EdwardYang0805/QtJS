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
// Func:      槽函数 注册Qt的对象以便HTML可以使用
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
// Func:      对上层提供自己添加HTML的方法
// Qualifier: btz
// Parameter: QString urlStr  html所在路径
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
// Func:      定时器槽函数，定时检查请求是否处理超时，超时立即返回错误
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
// Func:      加载html的槽函数，触发信号，上层可根据参数bSuccess判断加载HTML是否成功
// Qualifier: btz
// Parameter: bool bSuccess 代表加载html是否成功
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
// Func:      处理JS端的请求，发出信号，由上层处理，不是对Qt端提供的接口。
// Qualifier: btz
// Parameter: QString jsonStr JS端传来的请求格式是json字符串
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
// Returns:   int  0代表响应请求成功 -1代表失败
// Func:      响应JS请求，将上层获取的数据传回HTML
// Qualifier: btz
// Parameter: QString url				指令
// Parameter: int state					状态码
// Parameter: QString data				返回数据
// Parameter: QString dataType			数据类型
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
// Func:      Qt端主动推送信号或数据的接口
// Qualifier: btz
// Parameter: QString url			指令
// Parameter: QString dataType		数据类型
// Parameter: QString data			推送的数据
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