// fiveutils.h
#ifndef FIVEUTILS_H
#define FIVEUTILS_H

#include <string>

using namespace std;

const int FIVE_DESC_WIDTH = 13;
const int FIVE_DESC_HEIGHT = 10;

const int FIVE_COMPLETE_ESTIMATE = 100000;
const int FIVE_BLOCKED_ESTIMATE = -10000;

enum CellColor {
    EmptyColor,
    RedColor,
    BlueColor
};


void showMsg(const string& msg);

#endif // FIVEUTILS_H
