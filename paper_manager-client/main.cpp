#include "MainWindow.h"
#include "LoginWindow.h"
#include "ErrorWindow.h"
#include "PSQLinterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PSQLInterface db;
    MainWindow w(&db);
    LoginWindow l(&db, &w);
    QObject::connect(&l, &LoginWindow::login_successful,
                    &w, &MainWindow::init);
    l.show();
    return a.exec();
}
