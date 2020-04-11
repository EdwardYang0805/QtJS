#include "qtalternatehtml.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtAlternateHtml w;
	w.show();
	return a.exec();
}
