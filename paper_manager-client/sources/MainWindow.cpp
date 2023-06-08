#include "MainWindow.h"
#include "PSQLinterface.h"
#include "ItemViewWindow.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

MainWindow::MainWindow(PSQLInterface *psqli) 
    : BaseWindow(psqli, "Публикации"){
    this->resize(800, 500);

    tabsWidget = new QTabWidget();
    
    pn_tab = new QWidget();
    pn_table = create_table(pn_tab);
    tabsWidget->addTab(pn_tab, QString("Публикации"));

    ar_tab = new QWidget;
    ar_table = create_table(ar_tab);
    tabsWidget->addTab(ar_tab, QString("Авторы"));

    pr_tab = new QWidget;
    pr_table = create_table(pr_tab);
    tabsWidget->addTab(pr_tab, QString("Издательства"));

    cn_tab = new QWidget;
    cn_table = create_table(cn_tab);
    tabsWidget->addTab(cn_tab, QString("Сборники"));

    connect(tabsWidget, &QTabWidget::currentChanged, this, [=](int index){
    switch (index)
        {
        case 0:
            update_table(pn_table, "SELECT * FROM PUBLICATION_COMPOSITE", 
            {"Название", "Тип", "Дата", "Авторы", "Издательство"});
            break;
        case 1:
            update_table(ar_table, "SELECT * FROM AUTHOR_COMPOSITE", 
            {"ФИО", "Науч. степень", "Статьи", "Препринты", "Монографии", "Диссертации", "Патенты", "Отчеты"});
            break;
        case 2:
            update_table(pr_table, "SELECT * FROM PUBLISHER", 
            {"Название", "Аббревиатура", "Страна", "Город", "Адрес", "Контактный номер", "Эл. почта"});
            break;
        case 3:
            update_table(cn_table, "SELECT * FROM COMPILATION_COMPOSITE", 
            {"Название", "Дата", "Издательство", "Публикации"});
            break;
        }
    });

    centralL->addWidget(tabsWidget);  

    if (can_edit){
        addBtn = new QPushButton();
        addBtn->setText("+");
        QHBoxLayout* addBtnL = new QHBoxLayout();
        addBtn->setFixedSize(30, 30);
        addBtn->setStyleSheet("background-color: #5eba7d; color: white; font-size: 25px;");
        QSpacerItem* addBtnSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        addBtnL->addSpacerItem(addBtnSpacer);
        addBtnL->addWidget(addBtn);
        centralL->addLayout(addBtnL);
        connect(addBtn, &QPushButton::clicked,
                this, [=](){create_itemview(-1);});
    }

    tabsWidget->setCurrentIndex(1);
    tabsWidget->setCurrentIndex(0);
    show();
}

QTableView* MainWindow::create_table(QWidget* parent){
    QTableView* table = new QTableView();
    table->verticalHeader()->hide();
    table->setColumnHidden(0, true);
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(table);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(table, &QTableView::doubleClicked,
            this, [=](){itemview_requested(table);});
    return table;
}

void MainWindow::update_table(QTableView* table, QString query, QStringList labels){
    delete table->model();
    table->setModel(query_database(query));
    table->setColumnHidden(0, true);
    for (int i = 0; i < labels.size(); ++i){
        table->model()->setHeaderData(i+1, Qt::Horizontal, labels.at(i));
    }
}

void MainWindow::itemview_requested(QTableView* table){
    int id = table->selectionModel()->selectedRows(0)[0].data().toInt();
    create_itemview(id);
}

void MainWindow::create_itemview(int id){
    switch (this->tabsWidget->currentIndex())
    {
    case 0:
        bind_itemview_window(new PublicationViewWindow(db, this, id));
        break;
    case 1:
        bind_itemview_window(new AuthorViewWindow(db, this, id));
        break;
    case 2:
        bind_itemview_window(new PublisherViewWindow(db, this, id));
        break;
    case 3:
        bind_itemview_window(new CompilationViewWindow(db, this, id));
        break;
    default:
        break;
    }
}

void MainWindow::bind_itemview_window(ItemViewWindow* window){
    disable();
    subwindows.push_back(window);
    connect(window, &PublicationViewWindow::window_closed, this, [=](){
        this->enable();
        int n =this->tabsWidget->currentIndex();
        tabsWidget->setCurrentIndex(0);
        tabsWidget->setCurrentIndex(n);
    });
}
