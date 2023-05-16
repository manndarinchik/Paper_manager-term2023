#include "basewindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

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

QSqlQueryModel* BaseWindow::query_database(const char *ch)
{
    QSqlQueryModel* model = db->issue_query(QString(ch));
    QSqlError error = model->lastError();
    switch (error.type())
    {
    case QSqlError::NoError:
        return model; break;
    case QSqlError::ConnectionError:
        show_error("Не удалось подключиться к базе данных"); break;
    case QSqlError::StatementError:
        show_error("Неверный запрос к базе данных, обратитесь к администратору."); break;
    case QSqlError::TransactionError:
        show_error("Ошибка транзакции, обратитесь к администратору."); break;
    case QSqlError::UnknownError:
        show_error("Неизвестная ошибка, обратитесь к администратору."); break;
    default:
        break;
    }
    delete model;
    return nullptr;
}