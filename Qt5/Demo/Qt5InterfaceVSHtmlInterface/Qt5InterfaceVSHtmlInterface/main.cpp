#include "Qt5InterfaceVSHtmlInterface.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt5InterfaceVSHtmlInterface w;
    w.show();
    return a.exec();
}
