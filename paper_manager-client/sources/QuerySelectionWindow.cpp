#include "QuerySelectionWindow.h"
#include <QHeaderView>
#include <QAbstractItemView>

QuerySelectionWindow::QuerySelectionWindow(PSQLInterface* psqli, QWidget *parent, QString query, std::vector<int> hidden_colums)
    : BaseWindow(psqli, "Выбор значения", parent){
    this->setFixedSize(400, 500);
    //qDebug() << query;
    table = new QTableView(this);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    confirm = new QPushButton("Подтвердить", this);
//     create_value = new QPushButton("Создать", this);
//     centralL->addWidget(create_value);
    centralL->addWidget(table);
    centralL->addWidget(confirm);

    model = query_database(query);
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for (int i = 0; i < hidden_colums.size(); ++i)
        table->setColumnHidden(hidden_colums.at(i), true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(table, &QTableView::doubleClicked,
            this, &QuerySelectionWindow::confirm_selection);
    connect(confirm, &QPushButton::clicked,
            this, &QuerySelectionWindow::confirm_selection);
            
//     connect(create_value, &QPushButton::clicked,
//             this, [=](){emit value_create_request();});

    show();
}

void QuerySelectionWindow::confirm_selection(){
    if (table->selectionModel()->hasSelection())
        emit selection_confirmed(table->selectionModel()->selectedRows(0)[0]); 
}