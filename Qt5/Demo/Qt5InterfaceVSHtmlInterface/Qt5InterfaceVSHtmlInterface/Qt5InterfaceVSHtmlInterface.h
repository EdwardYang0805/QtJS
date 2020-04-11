#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt5InterfaceVSHtmlInterface.h"
#include "QtWeb.h"

class Qt5InterfaceVSHtmlInterface : public QMainWindow
{
    Q_OBJECT

public:
    Qt5InterfaceVSHtmlInterface(QWidget *parent = Q_NULLPTR);
public slots:
	void doResponseRequest(QString, QString);
	void domsgLoadFinish(bool);
	void domsgClick();

private:
    Ui::Qt5InterfaceVSHtmlInterfaceClass ui;
	QtWeb *m_pQtWeb;
	bool m_loadHtmlIsOk;
};
