#include "fiveaboutdlg.h"
#include "ui_fiveaboutdlg.h"

FiveAboutDlg::FiveAboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiveAboutDlg)
{
    ui->setupUi(this);
    QPixmap pix("FiveLogo.png");
    ui->imgLB->setPixmap(pix);
}


FiveAboutDlg::~FiveAboutDlg()
{
    delete ui;
}

void FiveAboutDlg::on_okPB_clicked()
{
    close();
}
