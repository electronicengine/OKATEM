#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <QApplication>

#include "enum.h"
#include "monad.h"
#include "cint.h"
#include "mainwindow.h"



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

     return a.exec();
}
