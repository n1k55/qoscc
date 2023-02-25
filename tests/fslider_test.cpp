#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QSlider>
#include <QLayout>
#include "fslider.h"
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto* test_fslider = new fSlider("testslider");
    QMainWindow w;

    auto *layout = new QHBoxLayout;
    layout->addWidget(test_fslider);

    // Set layout in QWidget
    auto *slider_container = new QWidget();
    slider_container->setLayout(layout);

    w.setCentralWidget(slider_container);
    w.show();
 
    return QApplication::exec();
}