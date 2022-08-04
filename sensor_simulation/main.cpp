#include "simplesimulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleSimulation w;
    w.show();
    return a.exec();
}
