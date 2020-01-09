#include "settingsdlg.h"
#include "ui_settingsdlg.h"

void SettingsDB::fromBuffer(const char* buf)
{
    mColor = (CellColor)(buf[0] - '0');
    mLevel = (int)(buf[1] - '0');
}

// buf - preallocated buffer by size = 2
void SettingsDB::toBuffer(char* buf)
{
    buf[0] = (char)mColor + '0';
    buf[1] = (char)mLevel + '0';
}

SettingsDlg::SettingsDlg(SettingsDB *settingsData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg),
    mData(settingsData)
{
    ui->setupUi(this);

    bool red = (mData->getColor() == RedColor);
    ui->redRadioBtn->setChecked(red);
    ui->blueRadioBtn->setChecked(red ? false : true);

    int level = mData->getLevel();
    ui->beginnerRadioBtn->setChecked(level == 0);
    ui->profiRadioBtn->setChecked(level == 1);
    ui->masterRadioBtn->setChecked(level == 2);
}

SettingsDlg::~SettingsDlg()
{
    delete ui;
}

void SettingsDlg::on_okPB_clicked()
{
    CellColor color = (ui->redRadioBtn->isChecked()) ? RedColor : BlueColor;

    int level = 2;
    if (ui->beginnerRadioBtn->isChecked()) level = 0;
    else if (ui->profiRadioBtn->isChecked()) level = 1;
    else level = 2;

    mData->set(color, level);
    close();
}

void SettingsDlg::on_cancelPB_clicked()
{
    close();
}
