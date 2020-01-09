// fiveseqstorage.h
#ifndef FIVESEQSTORAGE
#define FIVESEQSTORAGE_H

#include <vector>
#include <QPoint>
#include <fiveestimate.h>

using namespace std;

// storage for one sequence (chain in 5 row/column/diagonal of cells)
////////////////////////////////////////////////////////////////////////
class SeqStorage
{
    FiveEstimateData mEstimateData;
    vector<QPoint> mPositions;

public:
    SeqStorage(FiveEstimateData* estimateData) :
        mEstimateData(*estimateData, false) {}
    SeqStorage(const SeqStorage&storage);
    bool hasSame(const SeqStorage *seq);
    int getEstimation() const { return mEstimateData.getEstimate(); }
    void addPos(int x, int y);
    void printInfo(ofstream& logFile);
};

// Manager of data base of the SeqStorage
////////////////////////////////////////////////////////////////////////
class SeqStorageManager
{
    vector<SeqStorage*> mSeqArr;

public:
    ~SeqStorageManager();
    int getEstimate();
    void addSeqStorage(SeqStorage* seq, bool check_same = true);
    void clean();
    void printInfo(ofstream& logFile);
    SeqStorageManager& operator += (const SeqStorageManager&mng);
};

#endif // FIVESEQSTORAGE_H
