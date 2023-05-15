#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include "psqlinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(PSQLInterface *psqli, QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void login_successful();

private slots:
    void on_login_clicked();

private:
    PSQLInterface *db;
    QLineEdit *login_input;
    QLineEdit *pswd_input;
    QPushButton *login_btn;
    QLabel *login_error ;
};
#endif // MAINWINDOW_H
