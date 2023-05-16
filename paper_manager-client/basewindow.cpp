#include "basewindow.h"
#include <QVBoxLayout>

BaseWindow::BaseWindow(PSQLInterface *psqli, const char *ch, QWidget *parent)
    : QMainWindow()
{
    db = psqli;
    this->setWindowTitle(QString(ch));
    QWidget *centralW = new QWidget(this);
    this->setCentralWidget(centralW);
    centralL = new QVBoxLayout(centralW);

    errorWindow = new ErrorWindow(this);
    QObject::connect(this, &BaseWindow::show_error,
                    errorWindow, &ErrorWindow::invoke_window);
    QObject::connect(errorWindow, &ErrorWindow::message_shown,
                    this, &BaseWindow::disable);
    QObject::connect(errorWindow, &ErrorWindow::message_closed,
                    this, &BaseWindow::enable);
}
BaseWindow::~BaseWindow(){}