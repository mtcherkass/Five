// settingsdlg.h

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include <fiveutils.h>

const int FIVE_SETTINGS_DATA_SIZE = 2;

namespace Ui {
class SettingsDlg;
}

class SettingsDB
{
    CellColor mColor;
    int mLevel;

public:
    SettingsDB() : mColor(RedColor), mLevel(1) {}
    void set(CellColor color, int level) { mColor = color, mLevel = level; }
    CellColor getColor() const { return mColor; }
    int getLevel() const { return mLevel; }
    void fromBuffer(const char* buf);
    void toBuffer(char* buf);
    SettingsDB& operator=(const SettingsDB& data)
    {
        set(data.mColor, data.mLevel);
        return *this;
    }
};

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(SettingsDB* settingsData, QWidget *parent = 0);
    ~SettingsDlg();

private slots:
    void on_okPB_clicked();

    void on_cancelPB_clicked();

private:
    Ui::SettingsDlg *ui;
    SettingsDB* mData;
};


#endif // SETTINGSDLG_H
