#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "MainWindow.h"
#include "LoginWindow.h"
#include "ErrorWindow.h"
#include "PSQLinterface.h"
#include <QApplication>

class MainWorker : public QObject{
    Q_OBJECT
private:
    PSQLInterface* db;
    MainWindow* w;
    LoginWindow* l;
public:
    MainWorker(){
        db = new PSQLInterface();
        l = new LoginWindow(db);
        l->show();
        connect(l, &LoginWindow::login_successful,
                this, &MainWorker::start_main_window);
    }
    ~MainWorker(){
        delete w;
        delete db;
    }
private slots:
    void start_main_window(){
        w = new MainWindow(db);
        delete l;
    }
};
#endif //MAINWORKER_H