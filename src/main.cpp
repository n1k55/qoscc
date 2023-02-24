#include <QtWidgets/QApplication>
#include "qosccwindow.h"
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QOscCWindow w;
    w.show();
 
    return QApplication::exec();
}