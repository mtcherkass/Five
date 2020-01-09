#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <settingsdlg.h>
#include <fivedata.h>
#include <fiveaboutdlg.h>

const unsigned int AREA_X = 27;
const unsigned int AREA_Y = 55;
const unsigned int CELL_SIZE = 50;

const unsigned int SCREEN_HEIGHT = 500;
const unsigned int SCREEN_WIDTH = 650;

const unsigned int CELL_MAP_WIDTH = SCREEN_WIDTH / CELL_SIZE;
const unsigned int CELL_MAP_HEIGHT = SCREEN_HEIGHT / CELL_SIZE;

const int FIVE_MAX_STEPS_COUNT = 30;

class FiveCell;

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPushButton* getServicePB();

private slots:
    void on_servicePB_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    void init();
    void initMenu();
    void loadSettings();
    void saveSettings();
    bool getBtnPos(QPushButton* pb, QPoint& pos);
    void drawStepImage(QPushButton* pushBtn, int color, const QPoint& pos, bool smiley);
    void cleanBoard();
    void initCell(int x, int y);
    void activateEndOfGame(int endGameReason, CellColor color);
    void on_newGamePB_clicked();
    void on_settingsPB_clicked();
    void on_About_clicked();
    void on_Rules_clicked();
    void on_Exit_clicked();
    CellColor getUserColor() { return mSettingsData.getColor(); }
    int getLevel() { return mSettingsData.getLevel(); }

    Ui::MainWindow *ui;
    QGraphicsScene* mScene;
    QGraphicsPixmapItem* mRed;
    QGraphicsPixmapItem* mBlue;
    QPushButton* mCellMap[CELL_MAP_HEIGHT][CELL_MAP_WIDTH];
    QSignalMapper* signalMapper;
    SettingsDB mSettingsData;
    SettingsDB mUpdatedSettingsData;
    FiveData mData;
    bool mStarted;
    int mStepsCounter;
};


#endif // MAINWINDOW_H
