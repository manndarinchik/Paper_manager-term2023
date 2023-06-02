#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "basewindow.h"
#include "psqlinterface.h"

class MainWindow : public BaseWindow
{
    Q_OBJECT

public:
    MainWindow(PSQLInterface* psqli);

private:
    QWidget *pn_tab;
    QWidget *ar_tab;
    QWidget *pr_tab;
    QWidget *cn_tab;
    QTableView *pn_table;
    QTableView *ar_table;
    QTableView *pr_table;
    QTableView *cn_table;
    QPushButton *addBtn;

    QTableView* create_table(QWidget* parent); 

signals:

public slots:
    void init();

private slots:
    void update_tab_table(int index);
    void show_publication_window(int id = -1);
    void edit_publication(const QModelIndex &index);
    void delete_child_window(QMainWindow* ptr);
};
#endif // MAINWINDOW_H
