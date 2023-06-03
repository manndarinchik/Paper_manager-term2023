#include "LoginWindow.h"
#include "PSQLinterface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

LoginWindow::LoginWindow(PSQLInterface *psqli, QWidget *parent)
    : BaseWindow(psqli, "Вход", parent)
{
    this->resize(300, 200);

    QHBoxLayout *login_lo = new QHBoxLayout();
    QLabel *login_label = new QLabel("Логин:");
    login_input = new QLineEdit();
    login_input->setObjectName("login_input");
    login_lo->addWidget(login_label);
    login_lo->addWidget(login_input);

    QHBoxLayout *pswd_lo = new QHBoxLayout();
    QLabel *pswd_label = new QLabel(QString("Пароль:"));
    pswd_input = new QLineEdit();
    pswd_input->setObjectName("pswd_input");
    pswd_input->setEchoMode(QLineEdit::Password);
    pswd_lo->addWidget(pswd_label);
    pswd_lo->addWidget(pswd_input);

    QHBoxLayout *btn_lo = new QHBoxLayout();
    login_btn = new QPushButton(QString("Войти"));
    login_btn->setObjectName("login_btn");
    login_btn->setFixedWidth(100);
    QObject::connect(login_btn, &QPushButton::clicked,
                     this, &LoginWindow::on_login_clicked);

    btn_lo->addWidget(login_btn);

    centralL->setContentsMargins(25, 50, 25, 50);
    centralL->addLayout(login_lo);  
    centralL->addLayout(pswd_lo);  
    centralL->addLayout(btn_lo);

    this->setFixedSize(sizeHint());
}

void LoginWindow::on_login_clicked(){
    QString login = login_input->text();
    QString pswd = pswd_input->text();

    db->connect_to_bd(login, pswd);
    qDebug() << login << pswd << db->isOpen();

    if (db->isOpen()){
        close();
        login_successful();
    } else {
        emit show_error(QString("Ошибка авторизации"));
    }
}

