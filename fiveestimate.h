// fiveestimate.h
#ifndef FIVEESTIMATE_H
#define FIVEESTIMATE_H

#include <fiveutils.h>

struct FiveEstimateData
{
    FiveEstimateData(CellColor color, bool isMyColor, int checkDepth) :
        mColor(color), mIsMyColor(isMyColor), mCheckDepth(checkDepth)
    {
        setEstimate(0, 0);
    }

    FiveEstimateData(const FiveEstimateData& data, bool resetEstimates = false)
    {
        mColor = data.mColor;
        mIsMyColor = data.mIsMyColor;
        mCheckDepth = data.mCheckDepth;
        int myEstimate = (resetEstimates) ? 0 : data.mMyEstimate;
        int oppEstimate = (resetEstimates) ? 0 : data.mOpponentEstimate;
        setEstimate(myEstimate, oppEstimate);
    }

    void setEstimate(int myEstimate, int oppEstimate)
    {
        mMyEstimate = myEstimate;
        mOpponentEstimate = oppEstimate;
    }

    int getEstimate() const { return (mIsMyColor) ? mMyEstimate : mOpponentEstimate; }
    CellColor getColor() const { return mColor; }
    int getCheckDepth() const { return mCheckDepth; }
    void printInfo(ofstream& logFile);

private:
    CellColor mColor;
    bool mIsMyColor;
    int mMyEstimate;
    int mOpponentEstimate;
    int mCheckDepth;
};

#endif // FIVEESTIMATE_H
