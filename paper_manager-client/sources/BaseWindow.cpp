#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include "BaseWindow.h"
#include <QRegExp>

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
    if (!error.isValid())
        return model;
    
    QRegExp rx("\n");
    rx.setPatternSyntax(QRegExp::FixedString);
    int end = rx.indexIn(error.databaseText());
    qDebug() << end;
    QString error_text = error.databaseText().mid(6, end-6);

    if (error.type() == QSqlError::ConnectionError)
        emit show_error("Не удалось подключиться к базе данных");
    else 
        emit show_error(error_text);

    delete model;
    return nullptr;
}

void BaseWindow::closeEvent(QCloseEvent *event){
    //qDebug() << "Closing window";
    for (int i = 0; i < subwindows.size(); ++i){
        if (subwindows.at(i) != nullptr)
            subwindows.at(i)->close();
    }
    event->accept();
    emit window_closed(); 
}