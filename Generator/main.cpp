#include <QApplication>

#include "simulator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    dt::widgets::Simulator _simulator;
    _simulator.show();

    return a.exec();
}
