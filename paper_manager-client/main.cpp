#include "mainwindow.h"
#include "loginwindow.h"
#include "psqlinterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PSQLInterface db;
    MainWindow w(&db);
    LoginWindow l(&db);
    QObject::connect(&l, &LoginWindow::login_successful,
                    &w, &MainWindow::init);
    l.show();
    return a.exec();
}
