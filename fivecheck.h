// fivecheck.h

#ifndef FIVECHECK_H
#define FIVECHECK_H

#include <fivedata.h>

//////////////////////////////////////////////////
class ChecksIntf
{
public:
    ChecksIntf(FiveData* data) : mData(data) { }
    virtual ~ChecksIntf() {}
    virtual bool isLastCellLegal(int x, int y) = 0;
    virtual bool blockingCellCheck(int x, int y, int idx, CellColor inColor) = 0;
    virtual bool inColorCellCheck(int x, int y, int idx, CellColor inColor) = 0;
    virtual bool beforeFirstColorCheck(int x, int y, CellColor color) = 0;
    virtual bool afterLastColorCheck(int x, int y, CellColor color) = 0;

    bool beforeFirstEmptyCheck(int x, int y)
    {  return beforeFirstColorCheck(x, y, EmptyColor); }

    bool afterLastEmptyCheck(int x, int y)
    { return afterLastColorCheck(x, y, EmptyColor); }

    virtual int nextX(int x, int delta) = 0;
    virtual int nextY(int y, int delta) = 0;
protected:
    FiveData* mData;
};

//////////////////////////////////////////////////
struct XChecks : public ChecksIntf
{
    XChecks(FiveData* data) : ChecksIntf(data) {}
    bool blockingCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (!mData->isLegalX(x + i) ||
                mData->getValAt(y, x + i) == mData->getOppositeVal(inColor));
    }

    bool inColorCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (mData->getValAt(y, x + i) == inColor);
    }

    bool beforeFirstColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x-1, y, color);
    }

    bool afterLastColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x+5, y, color);
    }

    bool isLastCellLegal(int x, int)
    {
        return mData->isLegalX(x+4);
    }

    int nextX(int x, int delta) { return x + delta; }
    int nextY(int y, int) { return y; }
};

//////////////////////////////////////////////////
struct YChecks : public ChecksIntf
{
    YChecks(FiveData* data) : ChecksIntf(data) {}
    bool blockingCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (!mData->isLegalY(y + i) ||
                mData->getValAt(y + i, x) == mData->getOppositeVal(inColor));
    }

    bool inColorCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (mData->getValAt(y + i, x) == inColor);
    }

    bool beforeFirstColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x, y-1, color);
    }

    bool afterLastColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x, y+5, color);
    }

    bool isLastCellLegal(int,int y)
    {
        return mData->isLegalY(y+4);
    }

    int nextX(int x, int) { return x; }
    int nextY(int y, int delta) { return y + delta; }
};

//////////////////////////////////////////////////
struct RightDiagChecks : public ChecksIntf
{
    RightDiagChecks(FiveData* data) : ChecksIntf(data) {}
    bool blockingCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (!mData->isLegalY(y + i) || !mData->isLegalX(x + i) ||
                mData->getValAt(y+i, x+i) == mData->getOppositeVal(inColor));
    }

    bool inColorCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (mData->getValAt(y+i, x+i) == inColor);
    }

    bool beforeFirstColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x-1, y-1, color);
    }

    bool afterLastColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x+5, y+5, color);
    }

    bool isLastCellLegal(int x, int y)
    {
        return mData->isLegalX(x+4) && mData->isLegalY(y+4);
    }

    int nextX(int x, int delta) { return x + delta; }
    int nextY(int y, int delta) { return y + delta; }
};

//////////////////////////////////////////////////
struct LeftDiagChecks : public ChecksIntf
{
    LeftDiagChecks(FiveData* data) : ChecksIntf(data) {}
    bool blockingCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (!mData->isLegalY(y+i) || !mData->isLegalX(x-i) ||
                mData->getValAt(y+i, x-i) == mData->getOppositeVal(inColor));
    }

    bool inColorCellCheck(int x, int y, int i, CellColor inColor)
    {
        return (mData->getValAt(y+i, x-i) == inColor);
    }

    bool beforeFirstColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x+1, y-1, color);
    }

    bool afterLastColorCheck(int x, int y, CellColor color)
    {
        return mData->isCellHasColor(x-5, y+5, color);
    }

    bool isLastCellLegal(int x, int y)
    {
        return mData->isLegalX(x-4) && mData->isLegalY(y+4);
    }

    int nextX(int x, int delta) { return x - delta; }
    int nextY(int y, int delta) { return y + delta; }
};

#endif // FIVECHECK_H
