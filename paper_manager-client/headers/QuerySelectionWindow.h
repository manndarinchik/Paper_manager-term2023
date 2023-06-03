#ifndef QUERYSELECTIONWINDOW_H
#define QUERYSELECTIONWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QSqlQueryModel>
#include "BaseWindow.h"

class QuerySelectionWindow : public BaseWindow{
    Q_OBJECT
private:
    QTableView* table;
    QPushButton* confirm;
    QPushButton* create_value;
    QSqlQueryModel* model;
public:
    QuerySelectionWindow(PSQLInterface* psqli, QWidget *parent, QString query);
private slots:
    void confirm_selection();

signals:
    void selection_confirmed(QModelIndex selection);
    void value_create_request();
};

#endif // QUERYSELECTIONWINDOW_H