// fivedata.h

#ifndef FIVEDATA_H
#define FIVEDATA_H

#include <QPoint>
#include <vector>
#include <fiveseqstorage.h>
#include <fiveestimate.h>
#include <settingsdlg.h>

using namespace std;

class ChecksIntf;


class FiveData
{
public:
    FiveData(SettingsDB *settings);
    ~FiveData();
    bool calculateStep(QPoint& pos, CellColor myColor);
    bool isUserWin(CellColor myColor);
    static CellColor getOppositeVal(CellColor val) { return (val == RedColor) ? BlueColor : RedColor; }
    void setCellVal(const QPoint& pos, CellColor val);
    CellColor getValAt(int y, int x) { return mEntries[y][x]; }
    bool isLegalX(int x) { return (x >= 0 && x < FIVE_DESC_WIDTH); }
    bool isLegalY(int y) { return (y >= 0 && y < FIVE_DESC_HEIGHT); }
    bool isCellEmpty(int x, int y);
    bool isCellHasColor(int x, int y, CellColor color);
    vector<QPoint>& getVictoryPosArr() { return mVictoryPosArr; }

private:
    void findRandomMaxResultCell(int cellResArr[FIVE_DESC_HEIGHT][FIVE_DESC_WIDTH],
                                 QPoint& pos);
    void getRandomEmptyPos(QPoint& pos);
    bool isSequence(const vector<int>& arr);
    bool shouldSkipCheck(int matchCounter, bool isSeq, int checkDepth);
    int calculateEstimation(FiveEstimateData* estimateData, ofstream* logFile);
    void analizeSeqFromCell(int x, int y, FiveEstimateData* estimateData,
                            ChecksIntf* checks, SeqStorageManager *storageManager);
    void getEstimateByMatchSeq(const vector<int>& matchedArr, int x, int y,
                               ChecksIntf* checks, FiveEstimateData* estimateData,
                               SeqStorageManager* storageManager);
    const string& getFiveCharString(int num);
    void printLogHeader(ofstream& logFile);
    void setSeqEstimationData(int x, int y, int myEstimate, int oppEstimate,
                              const vector<int>& matchedArr, ChecksIntf* checks,
                              FiveEstimateData* estimateData, SeqStorageManager* storageManager);
    void printCheckInfo(ofstream* logFilePtr, SeqStorageManager *storageManager,
                        FiveEstimateData *checkEstimateData);
    void saveVictoryPosArr(int x, int y, ChecksIntf* checks);

    CellColor mEntries[FIVE_DESC_HEIGHT][FIVE_DESC_WIDTH];
    ChecksIntf* mChecks[4];
    SettingsDB* mSettings;
    vector<QPoint> mVictoryPosArr;
};


#endif // FIVEDATA_H
