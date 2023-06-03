#include "QuerySelectionWindow.h"
#include <QHeaderView>
#include <QAbstractItemView>

QuerySelectionWindow::QuerySelectionWindow(PSQLInterface* psqli, QWidget *parent, QString query)
    : BaseWindow(psqli, "Выбор значения", parent){
    this->setFixedSize(400, 500);
    //qDebug() << query;
    table = new QTableView(this);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    confirm = new QPushButton("Подтвердить", this);
    create_value = new QPushButton("Создать", this);
    centralL->addWidget(create_value);
    centralL->addWidget(table);
    centralL->addWidget(confirm);

    model = query_database(query);
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setColumnHidden(0, true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(confirm, &QPushButton::clicked,
            this, &QuerySelectionWindow::confirm_selection);
    connect(create_value, &QPushButton::clicked,
            this, [=](){emit value_create_request();});

    show();
}

void QuerySelectionWindow::confirm_selection(){
    if (table->selectionModel()->hasSelection())
        emit selection_confirmed(table->selectionModel()->selectedRows(0)[0]); 
}