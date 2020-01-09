// fiveseqstorage.cpp

#include <fstream>
#include <fiveseqstorage.h>

SeqStorage::SeqStorage(const SeqStorage& storage) :
    mEstimateData(storage.mEstimateData), mPositions(storage.mPositions) { }

void SeqStorage::addPos(int x, int y)
{
    QPoint pos(x, y);
    mPositions.push_back(pos);
}

bool SeqStorage::hasSame(const SeqStorage* seq)
{
    for (QPoint myPoint : mPositions)
    {
        for (QPoint seqPoint : seq->mPositions)
        {
            if (myPoint == seqPoint)
                return true;
        }
    }
    return false;
}

void SeqStorage::printInfo(ofstream& logFile)
{
    logFile << "Estimate: " << to_string(mEstimateData.getEstimate()) << ".   ";
    for (QPoint myPoint : mPositions)
    {
        logFile << "(" << to_string(myPoint.x()) << "," <<
                   to_string(myPoint.y()) << ")   ";
    }

    logFile << endl;
}

//*********************************************************
//*********************************************************
// check_same == true, check for same storage and don't add it if found
// check_same == false, don't check for same storage. Add it anyway
void SeqStorageManager::addSeqStorage(SeqStorage* seq, bool check_same)
{
    int size = mSeqArr.size();
    bool hasSame = false;

    if (check_same)
    {
        for (int i = 0; i < size; i++)
        {
            if (mSeqArr[i]->hasSame(seq))
            {
                hasSame = true;
                if (seq->getEstimation() > mSeqArr[i]->getEstimation())
                {
                    mSeqArr.erase(mSeqArr.begin() + i);
                    mSeqArr.push_back(seq);
                }
                // otherwise do nothing:
                // don't add the new sequence because we have already one better
            }
        }
    }

    if (!hasSame)
    {
        mSeqArr.push_back(seq);
    }
}

int SeqStorageManager::getEstimate()
{
    int estimate = 0;
    for (SeqStorage* seq : mSeqArr)
    {
        estimate += seq->getEstimation();
    }
    return estimate;
}

void SeqStorageManager::clean()
{
    for (SeqStorage* seq : mSeqArr) delete seq;
}

SeqStorageManager::~SeqStorageManager()
{
    clean();
}

SeqStorageManager& SeqStorageManager::operator += (const SeqStorageManager& mng)
{
    for (SeqStorage* seq : mng.mSeqArr)
    {
        addSeqStorage(new SeqStorage(*seq), false);
    }

    return *this;
}

void SeqStorageManager::printInfo(ofstream& logFile)
{
    for (SeqStorage* seq : mSeqArr)
    {
        seq->printInfo(logFile);

    }
}
