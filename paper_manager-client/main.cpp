#include <QApplication>
#include "MainWorker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWorker m;
    return a.exec();
}
