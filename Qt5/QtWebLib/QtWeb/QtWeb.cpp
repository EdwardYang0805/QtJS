#include "QtWeb.h"
#include <QWebChannel>
#include "Qt5WebEnginePage.h"
#include <qtimer.h>

using namespace std;

QtWeb::QtWeb(QWidget *parent)
{
	QTimer *checkTimer = new QTimer(this);
	connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkTimeout()));
	checkTimer->start(1000);
	m_pEnginePage = new Qt5WebEnginePage(this);
	this->setPage(m_pEnginePage);
	QWebChannel *channel = new QWebChannel(this);
	channel->registerObject(QStringLiteral("QtWebObj"), this);
	m_pEnginePage->setWebChannel(channel);
	connect(this, &QWebEngineView::loadFinished, [this](int ok) {
		dealLoadFinished(ok);
	});
}

QtWeb::~QtWeb()
{
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
		if ((oneItem["timestamp"].asInt() + oneItem["timeout"].asInt() / 1000) < QDateTime::currentDateTime().toTime_t())
		{
			responseInterFace(QString::fromStdString(oneItem["url"].asString()), 504, "error", "Text");
			m_requestList.removeOne(oneItem);
		}
	}
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
	if (reader.parse(jsonStr.toStdString().c_str(), root))
	{
		root["timestamp"] = QDateTime::currentDateTime().toTime_t();
		m_requestList.append(root);
		string url = root["url"].asString();
		string type = root["type"].asString();
		if (type != "POST")
			emit msgSendRequest(QString::fromStdString(url), "");
		else
		{
			string data = root["data"].asString();
			emit msgSendRequest(QString::fromStdString(url), QString::fromStdString(data));
		}
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
			m_pEnginePage->runJavaScript(QString("responseInterFace(%1)").arg(QString::fromLocal8Bit(jsonStr.c_str())));
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
	m_pEnginePage->runJavaScript(QString("webInterFace(%1)").arg(QString::fromLocal8Bit(jsonStr.c_str())));
}


