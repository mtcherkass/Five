#include "gameenddlg.h"
#include "ui_gameenddlg.h"
#include <string>

using namespace std;

GameEndDlg::GameEndDlg(int endReason, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameEndDlg)
{
    ui->setupUi(this);

    string img_name;
    QString title;
    switch (endReason)
    {
    case GAME_END_WIN:
        img_name = "winner200.png"; title = "YOU WIN !!!"; break;
    case GAME_END_LOSE:
        img_name = "loser200.png"; title = "Game lost"; break;
    case GAME_END_TIE:
        img_name = "holdinghands.png"; title = "Tie"; break;
    }

    QPixmap image(img_name.c_str());
    ui->ImageLB->setPixmap(image);

    this->setWindowTitle(title);
}

GameEndDlg::~GameEndDlg()
{
    delete ui;
}

void GameEndDlg::on_okPB_clicked()
{
    close();
}
