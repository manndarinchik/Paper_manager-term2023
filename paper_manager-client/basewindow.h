#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <psqlinterface.h>

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


public slots:
    void disable(){this->setEnabled(false);}
    void enable(){this->setEnabled(true);}
};

#endif