#pragma once

#include <QWebEngineView>
#include "json\json.h"

class Qt5WebEnginePage;

class QtWeb : public QWebEngineView
{
	Q_OBJECT

public:
	QtWeb(QWidget *parent);
	~QtWeb();
	public slots :
	void dealLoadFinished(bool);
	void dealRequest(QString jsonStr);
	int responseInterFace(QString url, int state, QString data, QString dataType = "json");
	void webInterFace(QString url, QString dataType = "json", QString data = "");
	void checkTimeout();
public:
	void loadUrl(QString urlStr);

signals:
	void msgLoadFinish(bool);
	void msgSendRequest(QString, QString);
	void msgRecieveJsonError(int); //参数为请求的错误码
private:
	Qt5WebEnginePage *m_pEnginePage;
	QList<Json::Value> m_requestList;
};
