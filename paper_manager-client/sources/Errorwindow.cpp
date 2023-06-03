#include "ErrorWindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QCloseEvent>

ErrorWindow::ErrorWindow(QWidget *parent)
    : QMainWindow(parent) 
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

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
    QPoint p = parentWidget()->pos();
    QPoint pCenter(parentWidget()->frameSize().width()/2, parentWidget()->frameSize().height()/2);
    QPoint sCenter(frameSize().width()/2, frameSize().height()/2);
    move(p + pCenter-sCenter);
    emit message_shown();
}

void ErrorWindow::close_message(){
    error_message->setText("");
    this->close(); 
}
    
void ErrorWindow::closeEvent(QCloseEvent *event)
{
      emit message_closed();
      event->accept();
}