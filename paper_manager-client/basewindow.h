#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <psqlinterface.h>
#include <QPushButton>
#include <QCloseEvent>
#include "errorwindow.h"

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseWindow(PSQLInterface *psqli, const char *ch = nullptr, QWidget *parent = nullptr);
    ~BaseWindow();

protected:
    PSQLInterface *db;
    QWidget *centralW;
    QVBoxLayout *centralL;
    ErrorWindow *errorWindow;

    QSqlQueryModel* query_database(const char *ch); 
    void closeEvent(QCloseEvent *event){emit window_closed(); event->accept();}

signals:
    void show_error(QString message);
    void window_closed();

public slots:
    void disable(){setEnabled(false); errorWindow->setEnabled(true);}
    void enable(){setEnabled(true);}
};

#endif