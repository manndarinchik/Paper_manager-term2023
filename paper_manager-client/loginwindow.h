#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "psqlinterface.h"
#include "basewindow.h"

class LoginWindow : public BaseWindow
{
    Q_OBJECT

public:
    LoginWindow(PSQLInterface *psqli, QWidget *parent);

signals:
    void login_successful();
    void error_called(QString message);

private slots:
    void on_login_clicked();
    

private:
    QLineEdit *login_input;
    QLineEdit *pswd_input;
    QPushButton *login_btn;
};
#endif // MAINWINDOW_H
