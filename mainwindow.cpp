#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fiveutils.h"
#include <fivegraphicsview.h>
#include <gameenddlg.h>
#include <fiverules.h>
#include <QtWidgets/QMessageBox>
#include <QStateMachine>
#include <QSignalMapper>
#include <string>
#include <fstream>

using namespace std;

const char* FIVE_SETTINGS_FILE = "five_settings.dat";


void FiveGraphicsView::mousePressEvent (QMouseEvent* /*event*/)
{
    /*
    const  QPointF pos = event->localPos();

    //int x = event->scenePos().x();

    string str = to_string((int)pos.x());
    str += "   ";
    str += (int)pos.y();
    showMsg(str);
    */
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mScene(nullptr),  mSettingsData(),
    mData(&mSettingsData), mStarted(false), mStepsCounter(0)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    loadSettings();
    mScene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(mScene);
    mScene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ui->servicePB->setVisible(false);

    for (unsigned int iY = 0; iY < CELL_MAP_HEIGHT; iY++)
    {
        for (unsigned int iX = 0; iX < CELL_MAP_WIDTH; iX++)
        {
            initCell(iX,iY); //mCellMap[iY][iX] = new FiveCell(this, iX, iY);
        }
    }

    initMenu();
}

void MainWindow::initMenu()
{
    connect(ui->actionAbout, &QAction::triggered, this,
            &MainWindow::on_About_clicked);

    connect(ui->actionRules, &QAction::triggered, this,
            &MainWindow::on_Rules_clicked);

    connect(ui->actionExit, &QAction::triggered, this,
            &MainWindow::on_Exit_clicked);

    connect(ui->actionNew_Game, &QAction::triggered, this,
            &MainWindow::on_newGamePB_clicked);

    connect(ui->actionSettings, &QAction::triggered, this,
            &MainWindow::on_settingsPB_clicked);
}

/////////////////////////////////
void MainWindow::loadSettings()
{
    ifstream settingsFile(FIVE_SETTINGS_FILE);

    if (settingsFile)
    {
        char buf[FIVE_SETTINGS_DATA_SIZE + 1];
        settingsFile.read (buf, FIVE_SETTINGS_DATA_SIZE + 1);
        mSettingsData.fromBuffer(buf);
        mUpdatedSettingsData = mSettingsData;
    }
}

/////////////////////////////////
void MainWindow::saveSettings()
{
    ofstream settingsFile;
    settingsFile.open (FIVE_SETTINGS_FILE, ios::out | ios::trunc );
    char buf[FIVE_SETTINGS_DATA_SIZE + 1];
    mUpdatedSettingsData.toBuffer(buf);

    // set last character to be null for creating a legal string
    buf[FIVE_SETTINGS_DATA_SIZE] = 0;
    settingsFile << buf;
}

/////////////////////////////////
void MainWindow::on_newGamePB_clicked()
{
    cleanBoard();
}

/////////////////////////////////
void MainWindow::on_settingsPB_clicked()
{
    SettingsDlg settingsDlg(&mUpdatedSettingsData);
    settingsDlg.setModal(true);
    settingsDlg.exec();

    // if still no move was done, we should apply settings changes
    if (!mStarted)
        mSettingsData = mUpdatedSettingsData;
}

/////////////////////////////////
void MainWindow::on_About_clicked()
{
    FiveAboutDlg aboutDlg;
    aboutDlg.setModal(true);
    aboutDlg.exec();
}

/////////////////////////////////
void MainWindow::on_Rules_clicked()
{
    FiveRules rulesDlg;
    rulesDlg.setModal(true);
    rulesDlg.exec();
}

/////////////////////////////////
void MainWindow::on_Exit_clicked()
{
    close();
}

/////////////////////////////////
QPushButton* MainWindow::getServicePB()
{
    return ui->servicePB;
}

////////////////

void MainWindow::initCell(int x, int y)
{
    mCellMap[y][x] = new QPushButton(this);
    QPushButton* btn = mCellMap[y][x];

    btn->move(AREA_X + CELL_SIZE * x, AREA_Y + CELL_SIZE * y);

    QSize size(CELL_SIZE, CELL_SIZE);
    btn->setFixedSize(size);

    QObject::connect(btn,
                     SIGNAL(clicked()),
                     this,
                     SLOT(on_servicePB_clicked()));

    btn->show();
}

//50 * 50

void MainWindow::on_servicePB_clicked()
{
    ++mStepsCounter;

     if (!mStarted)
     {
         mStarted = true;
         mSettingsData = mUpdatedSettingsData;
     }

     QObject* actionComponent = sender();

     if (actionComponent)
     {
         QPushButton* pushBtn = (QPushButton*)actionComponent;
         QPoint pressedBtnPos;
         if (getBtnPos(pushBtn, pressedBtnPos))
         {
            drawStepImage(pushBtn, getUserColor(), pressedBtnPos, false);
            mData.setCellVal(pressedBtnPos, getUserColor());

            if (mStepsCounter == FIVE_MAX_STEPS_COUNT)
            {
                activateEndOfGame(GAME_END_TIE, EmptyColor); // No winner
            }
            else if (mData.isUserWin(getUserColor()))
            {
                activateEndOfGame(GAME_END_WIN, getUserColor()); // user's victory !!!
            }
            else
            {
                ////////////////////////////
                /// App Answer
                ///
                QPoint appBtnPos;
                bool endOfGame = false;
                endOfGame = mData.calculateStep(appBtnPos, mData.getOppositeVal(getUserColor()));
                CellColor appColor = mData.getOppositeVal(getUserColor());
                QPushButton* calculatedPB =
                        mCellMap[appBtnPos.y()][appBtnPos.x()];

                drawStepImage(calculatedPB, appColor, appBtnPos, false);
                mData.setCellVal(appBtnPos, appColor);
                if (endOfGame)
                    activateEndOfGame(GAME_END_LOSE, appColor); // user's lose
            }
         }
     }
}

void MainWindow::activateEndOfGame(int endGameReason, CellColor color)
{   
    if (color != EmptyColor)
    {
        // draw smiley row
        vector<QPoint>& victoryPosArr = mData.getVictoryPosArr();
        for (QPoint pos : victoryPosArr)
        {
            drawStepImage(nullptr, color, pos, true);
        }
    }

    // disable all the buttons since game is over
    for (unsigned int iY = 0; iY < CELL_MAP_HEIGHT; iY++)
    {
        for (unsigned int iX = 0; iX < CELL_MAP_WIDTH; iX++)
        {
            mCellMap[iY][iX]->setEnabled(false);
        }
    }

    // show End Game dialog
    GameEndDlg gameEnd(endGameReason);
    gameEnd.exec();

    mStarted = false;
}

void MainWindow::cleanBoard()
{
    QPoint pos;
    for (unsigned int iY = 0; iY < CELL_MAP_HEIGHT; iY++)
    {
        for (unsigned int iX = 0; iX < CELL_MAP_WIDTH; iX++)
        {
            mCellMap[iY][iX]->setVisible(true);
            mCellMap[iY][iX]->setEnabled(true);
            pos.setX(iX);
            pos.setY(iY);
            mData.setCellVal(pos, EmptyColor);
        }
    }

    mSettingsData = mUpdatedSettingsData;
    mStepsCounter = 0;
}

void MainWindow::drawStepImage(QPushButton* pushBtn, int color, const QPoint& pos, bool smiley)
{
    if (pushBtn) pushBtn->setVisible(false);

    string imgName;
    if (smiley)
    {
        imgName = (color == RedColor) ? "red_smile.png" : "blue_smile.png";
    }
    else
    {
        imgName = (color == RedColor) ? "red.png" : "blue.png";
    }

    QPixmap pixmap(imgName.c_str());
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
    mScene->addItem(pixmapItem);
    pixmapItem->setPos(pos.x() * CELL_SIZE, pos.y() * CELL_SIZE);
    ui->graphicsView->show();
}

bool MainWindow::getBtnPos(QPushButton* pb, QPoint& pos)
{
    bool found = false;
    for (unsigned int iY = 0; iY < CELL_MAP_HEIGHT && !found; iY++)
    {
        for (unsigned int iX = 0; iX < CELL_MAP_WIDTH && !found; iX++)
        {
            if (mCellMap[iY][iX] == pb)
            {
                pos.setX(iX);
                pos.setY(iY);
                found = true;
            }
        }
    }
    return found;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
}
