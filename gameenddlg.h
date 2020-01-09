// gameenddlg.h
#ifndef GAMEENDDLG_H
#define GAMEENDDLG_H

#include <QDialog>
const int GAME_END_WIN = 0;
const int GAME_END_LOSE = 1;
const int GAME_END_TIE = 2;

namespace Ui {
class GameEndDlg;
}

class GameEndDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GameEndDlg(int endReason, QWidget *parent = 0);
    ~GameEndDlg();

private slots:
    void on_okPB_clicked();

private:
    Ui::GameEndDlg *ui;
};

#endif // GAMEENDDLG_H
