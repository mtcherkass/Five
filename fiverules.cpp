#include "fiverules.h"
#include "ui_fiverules.h"

FiveRules::FiveRules(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiveRules)
{
    ui->setupUi(this);
}

FiveRules::~FiveRules()
{
    delete ui;
}

void FiveRules::on_okPB_clicked()
{
    close();
}
