#include "mainwindow.h"
#include "fiveutils.h"
#include <QApplication>
#include <QMessageBox>

void showMsg(const string& msg)
{
    QMessageBox msgBox;
    QString qstr = msg.c_str();
    msgBox.setText(qstr);
    msgBox.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
