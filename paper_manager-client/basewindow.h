#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <psqlinterface.h>
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

signals:
    void show_error(QString message);

public slots:
    void disable(){setEnabled(false); errorWindow->setEnabled(true);}
    void enable(){setEnabled(true);}
};

#endif