#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    Widget w;

    QMenuBar mnuBar(&w);
    QMenu*   pmnu   = new QMenu("&Tools");

    pmnu->addAction("&Wave Viewert",
                    &w,
                    SLOT(slot_show()),
                    Qt::CTRL + Qt::Key_W );

    pmnu->addAction("&About Qt",
                    &a,
                    SLOT(aboutQt()),
                    Qt::CTRL + Qt::Key_A );

    pmnu->addSeparator();
    pmnu->addAction("&Exit",
                    &a,
                    SLOT(quit()),
                    Qt::CTRL + Qt::Key_Q);

    mnuBar.addMenu(pmnu);

    w.show();
    return a.exec();
}
