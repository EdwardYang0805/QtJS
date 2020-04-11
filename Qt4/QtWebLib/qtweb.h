#ifndef QTWEB_H
#define QTWEB_H

#include <QWebView>
#include "json/json.h"
using namespace std;

class QtWeb : public QWebView
{
	Q_OBJECT

public:
	QtWeb(QWidget *parent);
	~QtWeb();
	public slots:
		void dealLoadFinished(bool);
		void dealRequest(QString jsonStr);
		void registerJSObj();
		int responseInterFace(QString url, int state, QString data, QString dataType = "json");
		void webInterFace(QString url, QString dataType = "json", QString data = "");
		void checkTimeout();
public:
	void loadUrl(QString urlStr);

signals:
	void msgLoadFinish(bool);
	void msgSendRequest(QString, QString, QString);
	void msgRecieveJsonError(int); //参数为请求的错误码
private:
	QList<Json::Value> m_requestList;
};

#endif // QTWEB_H
