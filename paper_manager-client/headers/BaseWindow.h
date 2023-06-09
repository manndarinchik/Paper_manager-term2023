#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include "PSQLinterface.h"
#include <QPushButton>
#include <QCloseEvent>
#include "ErrorWindow.h"

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseWindow(PSQLInterface *psqli, const char *ch = nullptr, QWidget *parent = nullptr);
    ~BaseWindow(){};

protected:
    PSQLInterface *db;
    QWidget *centralW;
    QVBoxLayout *centralL;
    ErrorWindow *errorWindow;
    bool can_edit;
    std::vector<BaseWindow*> subwindows;

    QSqlQueryModel* query_database(QString query); 
    void closeEvent(QCloseEvent *event);

signals:
    void show_error(QString message);
    void window_closed();

public slots:
    void disable(){setEnabled(false); errorWindow->setEnabled(true);}
    void enable(){setEnabled(true);}
};

#endif