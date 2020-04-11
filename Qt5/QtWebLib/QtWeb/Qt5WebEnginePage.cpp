#include "Qt5WebEnginePage.h"

Qt5WebEnginePage::Qt5WebEnginePage(QObject *parent)
	: QWebEnginePage(parent)
{
}

Qt5WebEnginePage::~Qt5WebEnginePage()
{
}

bool Qt5WebEnginePage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
	return true;
}
