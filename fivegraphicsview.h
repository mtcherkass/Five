// fivegraphicsview.h

#ifndef FIVEGRAPHICSVIEW_H
#define FIVEGRAPHICSVIEW_H

#include <QGraphicsView>

class FiveGraphicsView : public QGraphicsView
{
public:
    FiveGraphicsView(QWidget* mainWin) : QGraphicsView(mainWin) {}

    void mousePressEvent (QMouseEvent*);
    //mouseReleaseEvent
};

#endif // FIVEGRAPHICSVIEW_H
