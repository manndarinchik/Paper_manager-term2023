#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include "BaseWindow.h"

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

    if (db->isOpen()){
        QSqlQueryModel* is_editor = query_database(QString("SELECT * FROM can_edit('%1')").arg(db->get_login()));
        can_edit = is_editor->data(is_editor->index(0, 0)).toBool();
        delete is_editor;
    } else
        can_edit = false;

    //qDebug() << "Editor window: "<<can_edit;
 
}

QSqlQueryModel* BaseWindow::query_database(QString query)
{
    QSqlQueryModel* model = db->issue_query(query);
    QSqlError error = model->lastError();
    qDebug() << "Query '" << model->query().lastQuery() << "': " << (!error.isValid() ? "OK" : error.databaseText());
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
    default:
        show_error("Неизвестная ошибка, обратитесь к администратору."); break;
    }
    delete model;
    return nullptr;
}