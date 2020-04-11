#pragma once

#include <QWebEnginePage>

class Qt5WebEnginePage : public QWebEnginePage
{
	Q_OBJECT

public:
	Qt5WebEnginePage(QObject *parent);
	~Qt5WebEnginePage();
protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
};
