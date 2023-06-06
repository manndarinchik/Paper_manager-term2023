#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "PSQLinterface.h"
#include "BaseWindow.h"

class LoginWindow : public BaseWindow
{
    Q_OBJECT

public:
    LoginWindow(PSQLInterface *psqli);

signals:
    void login_successful();

private slots:
    void on_login_clicked();
    
private:
    QLineEdit *login_input;
    QLineEdit *pswd_input;
    QPushButton *login_btn;
};
#endif // MAINWINDOW_H
