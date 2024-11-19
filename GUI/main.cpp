#include "mainwindow.h"
#include "startwindow.h".h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartWindow startw;
    startw.show();
    return a.exec();
}
