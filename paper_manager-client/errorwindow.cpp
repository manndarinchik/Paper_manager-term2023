#include "errorwindow.h"
#include <QVBoxLayout>
#include <QPushButton>

ErrorWindow::ErrorWindow(QWidget *parent)
    : QMainWindow(parent) 
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    resize(300, 100);
    setWindowTitle("Ошибка");
    QWidget *centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    error_message = new QLabel();

    QPushButton *close_btn = new QPushButton(QString("Понятно"));
    connect(close_btn, &QPushButton::clicked,
            this, &ErrorWindow::close_message);

    centralLayout->setContentsMargins(25, 50, 25, 50);
    centralLayout->addWidget(error_message);
    centralLayout->addWidget(close_btn);
}
ErrorWindow::~ErrorWindow(){
    delete error_message;
}

void ErrorWindow::invoke_window(QString message){
    error_message->setText(message);
    this->show();
    emit message_shown();
}

void ErrorWindow::close_message(){
    error_message->setText("");
    this->close(); 
    emit message_closed();
}
