// fiveaboutdlg.h
#ifndef FIVEABOUTDLG_H
#define FIVEABOUTDLG_H

#include <QDialog>

namespace Ui {
class FiveAboutDlg;
}

class FiveAboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FiveAboutDlg(QWidget *parent = 0);
    ~FiveAboutDlg();

private slots:
    void on_okPB_clicked();

private:
    Ui::FiveAboutDlg *ui;
};

#endif // FIVEABOUTDLG_H
