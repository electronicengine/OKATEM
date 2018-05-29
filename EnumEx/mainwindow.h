#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <tuple>

#include "enum.h"
#include "monad.h"
#include <fstream>
#include "widgetypes.h"
#include "cint.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);


    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

private:

      Ui::MainWindow *ui;




};

#endif // MAINWINDOW_H
