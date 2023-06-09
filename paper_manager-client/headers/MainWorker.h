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
        w = nullptr;
        db = nullptr;
        l = nullptr;
        start_login_window();
    }
    ~MainWorker(){
        if (w != nullptr) delete w;
        if (db != nullptr) delete db;
    }
private slots:
    void start_login_window(){
        db = new PSQLInterface();
        l = new LoginWindow(db);
        l->show();
        connect(l, &LoginWindow::login_successful,
                this, &MainWorker::start_main_window);
    }
    void start_main_window(){
        w = new MainWindow(db);
        connect(w, &MainWindow::logout, this, &MainWorker::reset_connection);
        delete l;
    }
    void reset_connection(){
        delete w;
        delete db;
        w = nullptr;
        db = nullptr;
        start_login_window();
    }
};
#endif //MAINWORKER_H