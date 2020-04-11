#ifndef QTALTERNATEHTML_H
#define QTALTERNATEHTML_H

#include <QMainWindow>
#include "ui_qtalternatehtml.h"
#include "qtweb.h"


class QtAlternateHtml : public QMainWindow
{
	Q_OBJECT

public:
	QtAlternateHtml(QWidget *parent = 0);
	~QtAlternateHtml();
public slots:
	void doResponseRequest(QString, QString, QString);
	void domsgLoadFinish(bool);
	void domsgClick();
private:
	Ui::QtAlternateHtmlClass ui;
	QtWeb *m_pQtWeb;
	bool m_loadHtmlIsOk;
};

#endif // QTALTERNATEHTML_H
