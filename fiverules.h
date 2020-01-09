// fiverules.h

#ifndef FIVERULES_H
#define FIVERULES_H

#include <QDialog>

namespace Ui {
class FiveRules;
}

class FiveRules : public QDialog
{
    Q_OBJECT

public:
    explicit FiveRules(QWidget *parent = 0);
    ~FiveRules();

private slots:
    void on_okPB_clicked();

private:
    Ui::FiveRules *ui;
};

#endif // FIVERULES_H
