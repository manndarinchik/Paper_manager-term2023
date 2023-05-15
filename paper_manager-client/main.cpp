#include "mainwindow.h"
#include "loginwindow.h"
#include "errorwindow.h"
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

    ErrorWindow er;
    QObject::connect(&l, &LoginWindow::error_called,
                    &er, &ErrorWindow::invoke_window);
    QObject::connect(&er, &ErrorWindow::message_shown,
                    &l, &LoginWindow::disable);
    QObject::connect(&er, &ErrorWindow::message_closed,
                    &l, &LoginWindow::enable);
    l.show();
    return a.exec();
}
