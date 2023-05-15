#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include "psqlinterface.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(PSQLInterface *psqli, QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void login_successful();
    void error_called(QString message);

public slots:
    void disable(){this->setEnabled(false);}
    void enable(){this->setEnabled(true);}

private slots:
    void on_login_clicked();
    

private:
    PSQLInterface *db;
    QLineEdit *login_input;
    QLineEdit *pswd_input;
    QPushButton *login_btn;
};
#endif // MAINWINDOW_H
