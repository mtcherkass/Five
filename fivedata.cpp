#include "fivedata.h"
#include "fivecheck.h"
#include "fiveutils.h"
#include <iostream>
#include <fstream>


FiveData::FiveData(SettingsDB* data) : mSettings(data)
{
    for (int xIdx = 0; xIdx < FIVE_DESC_WIDTH; xIdx++)
    {
        for (int yIdx = 0; yIdx < FIVE_DESC_HEIGHT; yIdx++)
        {
            mEntries[yIdx][xIdx] = EmptyColor;
        }
    }

    mChecks[0] = new XChecks(this);
    mChecks[1] = new YChecks(this);
    mChecks[2] = new RightDiagChecks(this);
    mChecks[3] = new LeftDiagChecks(this);
}

FiveData::~FiveData()
{
    for (ChecksIntf* check : mChecks) delete check;
}

void FiveData::setCellVal(const QPoint& pos, CellColor val)
{
    int x = pos.x();
    int y = pos.y();

    if (x > FIVE_DESC_WIDTH || x < 0)
    {
        string msg("Bad x value for setCellVal: ");
        msg += to_string(x);
        showMsg(msg);
    }

    if (y > FIVE_DESC_HEIGHT || y < 0)
    {
        string msg("Bad y value for setCellVal: ");
        msg += to_string(y);
        showMsg(msg);
    }

    mEntries[y][x] = val;
}

void FiveData::printLogHeader(ofstream& logFile)
{
    string entry;
    logFile << "            ";
    for (int i = 0; i < FIVE_DESC_WIDTH; i++)
    {
        entry = to_string(i) + string("                ");
        entry += (i<10) ? " " : "";

        logFile << entry;
    }
    logFile << endl;
}

// main calculating function
bool FiveData::calculateStep(QPoint& pos, CellColor myColor)
{
    bool endOfGame = false;
    int cellsEstimation[FIVE_DESC_HEIGHT][FIVE_DESC_WIDTH];

    ofstream logFile;
    logFile.open ("log.txt", ios::out | ios::trunc );
    printLogHeader(logFile);

    int appEstimation = 0, userEstimation = 0, userAfterStepEstimate = 0;
    FiveEstimateData appEstimateData(myColor, true, 0);
    FiveEstimateData userEstimateData(getOppositeVal(myColor), false, 0);
    FiveEstimateData userAfterStepEstimateData(getOppositeVal(myColor), true, 1);

    for (int yIdx = 0; yIdx < FIVE_DESC_HEIGHT; yIdx++)
    {
      for (int xIdx = 0; xIdx < FIVE_DESC_WIDTH; xIdx++)
      {
        if (mEntries[yIdx][xIdx] == EmptyColor)
        {
            mEntries[yIdx][xIdx] = myColor;
            appEstimation = calculateEstimation(&appEstimateData, &logFile);
            userEstimation = calculateEstimation(&userEstimateData, &logFile);

            mEntries[yIdx][xIdx] = getOppositeVal(myColor);
            userAfterStepEstimate =
                    calculateEstimation(&userAfterStepEstimateData, &logFile);
            mEntries[yIdx][xIdx] = EmptyColor;

            if (appEstimation >= FIVE_COMPLETE_ESTIMATE ||
                userEstimation >= FIVE_COMPLETE_ESTIMATE)
            {
                endOfGame = true;
            }

            // Limit userAfterStepEstimate value:
            // Reset the estimate for only one 3 sequence, and set maximum value for 300
            if (userAfterStepEstimate < 200) userAfterStepEstimate = 0;
            if (userAfterStepEstimate > 300) userAfterStepEstimate = 300;

            cellsEstimation[yIdx][xIdx] =
                    appEstimation - userEstimation + userAfterStepEstimate;
            string appEstimateStr  = getFiveCharString(appEstimation);
            string userEstimateStr = getFiveCharString(userEstimation);
            string userAfterStepEstimateStr = getFiveCharString(userAfterStepEstimate);
            logFile << appEstimateStr << "," << userEstimateStr << "," << userAfterStepEstimateStr << "  ";
        }
        else
        {
            // Not empty cell.
            // Set some very low estimation,
            // so it will never be considered as good placement
            cellsEstimation[yIdx][xIdx] = appEstimation = FIVE_BLOCKED_ESTIMATE;
            userEstimation = 0;
            logFile << "______BLOCK______  ";
        }
      }
      logFile << endl;
    }

    findRandomMaxResultCell(cellsEstimation, pos);
    logFile << "Max value is: " << cellsEstimation[pos.y()][pos.x()] <<
    ". At position x=" << pos.x() << ", y=" << pos.y() << endl;
    logFile.close();
    return endOfGame;
}

bool FiveData::isUserWin(CellColor myColor)
{
    FiveEstimateData estimateData(myColor, true, 0);
    return (bool)(calculateEstimation(&estimateData, nullptr) >= FIVE_COMPLETE_ESTIMATE);
}

// Find max results array and give random value from this array
void FiveData::findRandomMaxResultCell(int cellResArr[FIVE_DESC_HEIGHT][FIVE_DESC_WIDTH],
                                       QPoint& randPos)
{
    int max = FIVE_BLOCKED_ESTIMATE * 2;
    vector<QPoint> maxPosArr;

    // Calculate array of max values
    for (int yIdx = 0; yIdx < FIVE_DESC_HEIGHT; yIdx++)
    {
      for (int xIdx = 0; xIdx < FIVE_DESC_WIDTH; xIdx++)
      {
        if (cellResArr[yIdx][xIdx] > max)
        {
            QPoint pos(xIdx, yIdx);
            maxPosArr.clear();
            maxPosArr.push_back(pos);
            max = cellResArr[yIdx][xIdx];
        }
        else if (cellResArr[yIdx][xIdx] == max)
        {
            QPoint pos(xIdx, yIdx);
            maxPosArr.push_back(pos);
        }
      }
    }

    // get random entry from the max values array
    int idx = rand() % (int)maxPosArr.size();
    randPos = maxPosArr[idx];
}

// Provide estimation of current position for given estimateData(color, isUser..)
int FiveData::calculateEstimation(FiveEstimateData* estimateData,
                                  ofstream* logFile)
{
    SeqStorageManager sumStorageMng;

    for (ChecksIntf* check : mChecks)
    {
        SeqStorageManager storageManager;
        for (int yIdx = 0; yIdx < FIVE_DESC_HEIGHT; yIdx++)
        {
            for (int xIdx = 0; xIdx < FIVE_DESC_WIDTH; xIdx++)
            {
                FiveEstimateData checkEstimateData(*estimateData, true);
                analizeSeqFromCell (xIdx, yIdx, &checkEstimateData, check, &storageManager);


            }
        }
        printCheckInfo(logFile, &storageManager, estimateData);
        sumStorageMng += storageManager;
    }

    int estimate = sumStorageMng.getEstimate();
    return estimate;
}

void FiveData::printCheckInfo(ofstream* logFilePtr,
                              SeqStorageManager *,//storageManager,
                              FiveEstimateData *)//checkEstimateData)
{
    if (logFilePtr)
    {
        // TODOBOM
        //checkEstimateData->printInfo(*logFilePtr);
        //storageManager->printInfo(*logFilePtr);
    }
}

inline bool FiveData::isCellEmpty(int x, int y)
{
    return isCellHasColor(x, y, EmptyColor);
}

bool FiveData::isCellHasColor(int x, int y, CellColor color)
{
    if (!isLegalX(x) || !isLegalY(y))
        return false;

    return (mEntries[y][x] == color);
}

void FiveData::analizeSeqFromCell(int x, int y, FiveEstimateData* estimateData,
                                  ChecksIntf* checks, SeqStorageManager* storageManager)
{
    if (!checks->isLastCellLegal(x,y))
    {
        return;
    }

    bool blocked = false;
    CellColor inColor = estimateData->getColor();
    vector<int> matchedArr;

    // roll over the 5 cells and check for colored cells
    for (int i = 0; i < 5; i++)
    {
        if (checks->blockingCellCheck(x, y, i, inColor))
        {
            blocked = true; // has opposite color, hence blocking the seq
            break;
        }

        if (checks->inColorCellCheck(x, y, i, inColor))
        {
            matchedArr.push_back(i);
        }
    }
    if (!blocked)
    {
        // from this point each cell is empty or has inColor
        getEstimateByMatchSeq(matchedArr, x, y, checks, estimateData, storageManager);
    }
}
//////////////////////////////////////////////////////
/// save array of positions which won the game
void FiveData::saveVictoryPosArr(int x, int y, ChecksIntf* checks)
{
    mVictoryPosArr.clear();
    for (int i = 0; i < 5; i++)
    {
        QPoint p(checks->nextX(x, i), checks->nextY(y, i));
        mVictoryPosArr.push_back(p);
    }
}

//////////////////////////////////////////////////////
/// Should check of the sequence be skipped
bool FiveData::shouldSkipCheck(int matchCounter, bool isSeq, int checkDepth)
{
    // For the beginner and expert level don't check for 2 chain
    if ((matchCounter == 2) &&
        (mSettings->getLevel() == 0 || mSettings->getLevel() == 1)) return true;

    // For the begginer level skip 3 chain checks if it's not a seq
    if (mSettings->getLevel() == 0 && matchCounter == 3)
    {
        if (!isSeq) return true;
    }

    // Check second depth only for case of seq of 3 cells and only for Master level
    if (checkDepth == 1)
    {
        if (matchCounter != 3 || mSettings->getLevel() != 2) return true;
    }

    return false;
}

// Define estimate by given array of matched indexes for current check of (x,y)
// The estimate will be stored in the storageManager
void FiveData::getEstimateByMatchSeq(const vector<int>& matchedArr,
                                     int x, int y,
                                     ChecksIntf* checks,
                                     FiveEstimateData* estimateData,
                                     SeqStorageManager* storageManager)
{
    int matchCounter = matchedArr.size();
    bool isSeq = isSequence(matchedArr);

    if (shouldSkipCheck(matchCounter, isSeq, estimateData->getCheckDepth()))
    {
        return;
    }

    int myEst = 0, oppEst = 0;
    switch (matchCounter)
    {
    case 5:
        myEst = FIVE_COMPLETE_ESTIMATE;
        saveVictoryPosArr(x, y, checks);
        break;
    case 4:
        if (isSeq)
        {
            if ((matchedArr[0] == 0 && checks->afterLastEmptyCheck(x,y)) ||
                (matchedArr[0] == 4 && checks->beforeFirstEmptyCheck(x,y)))
            {
                // seq surrounded by empty cells
                myEst = 900; oppEst = 3600;
            }
            else
            {
                myEst = 450; oppEst = 2500;
            }
        }
        else
        {
            myEst = 400; oppEst = 2000;
        }
        break;
    case 3:
        if (isSeq)
        {
            // ***00
            if ((matchedArr[0] == 0 && matchedArr[2] == 2 && checks->beforeFirstEmptyCheck(x,y)) ||
            // 0***0
                (matchedArr[0] == 1 && matchedArr[2] == 3) ||
            // 00***
                (matchedArr[0] == 2 && matchedArr[2] == 4 && checks->afterLastEmptyCheck(x,y)))
            {
                myEst = 190; oppEst = 800;
            }
        }
        else
        {
            if (matchedArr[0] == 0 && matchedArr[2] == 4) // *???* - first is too far from last
            {
                myEst = 150; oppEst = 300;
            }
            else if (matchedArr[0] && checks->beforeFirstEmptyCheck(x,y))
            {
                myEst = 180; oppEst = 800;
            }
            else if (matchedArr[3] == 4 && checks->afterLastEmptyCheck(x,y))
            {
                myEst = 180; oppEst = 800;
            }
            else
            {
               myEst = 160; oppEst = 360;
            }
        }
        break;
    case 2:
        if (isSeq)
        {
            // we have 2 cells seq as a part of larger seq - skip its estimating
            if ((matchedArr[0] == 0 && checks->beforeFirstColorCheck(x,y,estimateData->getColor())) ||
                (matchedArr[1] == 4 && checks->afterLastColorCheck(x,y,estimateData->getColor()))) break;

            myEst = 120; oppEst = 200;
        }
        else
        {
            myEst = 100; oppEst = 180;
        }
        break;
    case 1: // usefull for first steps.
            // We should ensure that even when there is no strong step,
            // better estimate will be for setting near opponent cells.
        if (getValAt(checks->nextY(y, 1), checks->nextX(x, 1)) == EmptyColor)
            ++oppEst;

        if (getValAt(checks->nextY(y,-1), checks->nextX(x,-1)) == EmptyColor)
            ++oppEst;
        break;
    }

    if (myEst != 0 || oppEst != 0)
    {
        setSeqEstimationData(x, y, myEst, oppEst,
                             matchedArr, checks, estimateData, storageManager);
    }
}

void FiveData::setSeqEstimationData(int x, int y, int myEstimate, int oppEstimate,
                                    const vector<int>& matchedArr,
                                    ChecksIntf* checks,
                                    FiveEstimateData* estimateData,
                                    SeqStorageManager* storageManager)
{
    estimateData->setEstimate(myEstimate, oppEstimate);
    SeqStorage* seq = new SeqStorage(estimateData);
    for (int matched : matchedArr)
    {
        seq->addPos(checks->nextX(x, matched), checks->nextY(y, matched));
    }
    storageManager->addSeqStorage(seq);
}

// define is given array consist of sequence of integers
bool FiveData::isSequence(const vector<int>& arr)
{
    bool seq = true;
    for (int i = 0; i < (int)arr.size() - 1; i++)
    {
        if (arr[i] + 1 != arr[i+1])
        {
            seq = false;
            break;
        }
    }

    return seq;
}

// get string of 5 chars, reproducing a number in input
const string& FiveData::getFiveCharString(int num)
{
    static string str;

    str.clear();
    string numStr = to_string(num);
    int len = numStr.length();
    for (int i = 0; i < 5 - len; i++) str += "_";
    str += numStr;
    return str;
}

void FiveEstimateData::printInfo(ofstream& logFile)
{
        logFile << "Depth=" << mCheckDepth << ", mIsMyColor=" << mIsMyColor <<
                   ", mCheckDepth=" << mCheckDepth << "   :::" << endl;
}
