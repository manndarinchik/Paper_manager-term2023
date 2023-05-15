#include "mainwindow.h"
#include "psqlinterface.h"
#include <QVBoxLayout>
#include <QTableWidget>

MainWindow::MainWindow(PSQLInterface *psqli, QWidget *parent)
    : QMainWindow(parent){
        db = psqli;
}

void MainWindow::init(){
    this->resize(800, 500);
    this->setWindowTitle(QString("Публикации"));

    QWidget *centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QTabWidget *tabsWidget = new QTabWidget();
    QWidget *pn_tab = new QWidget;
    tabsWidget->addTab(pn_tab, QString("Публикации"));
    QVBoxLayout *pn_lo = new QVBoxLayout(pn_tab);
    QStringList pn_labels;
    pn_labels << "Название" << "Тип" << "Дата" << "Авторы" << "Издательство";
    QTableWidget* pn_table = new QTableWidget(0, pn_labels.length());
    pn_lo->addWidget(pn_table);
    pn_table->setHorizontalHeaderLabels(pn_labels);

    QWidget *ar_tab = new QWidget;
    tabsWidget->addTab(ar_tab, QString("Авторы"));
    QVBoxLayout *ar_lo = new QVBoxLayout(ar_tab);
    QStringList ar_labels;
    ar_labels << "ФИО" << "Положение" << "Статьи" << "Препринты" << "Монографии" << "Диссертации" << "Патенты" << "Отчеты";
    QTableWidget* ar_table = new QTableWidget(0, ar_labels.length());
    ar_lo->addWidget(ar_table);
    ar_table->setHorizontalHeaderLabels(ar_labels);

    QWidget *pr_tab = new QWidget;
    tabsWidget->addTab(pr_tab, QString("Издательства"));
    QVBoxLayout *pr_lo = new QVBoxLayout(pr_tab);
    QStringList pr_labels;
    pr_labels << "Название" << "Аббревиатура" << "Страна" << "Город" << "Адрес" << "Контактный номер" << "Эл. почта";
    QTableWidget* pr_table = new QTableWidget(0, pr_labels.length());
    pr_lo->addWidget(pr_table);
    pr_table->setHorizontalHeaderLabels(pr_labels);

    QWidget *cn_tab = new QWidget;
    tabsWidget->addTab(cn_tab, QString("Сборники"));
    QVBoxLayout *cn_lo = new QVBoxLayout(cn_tab);
    QStringList cn_labels;
    cn_labels << "Название" << "Дата" << "Издательство" << "Публикации";
    QTableWidget* cn_table = new QTableWidget(0, cn_labels.length());
    cn_lo->addWidget(cn_table);
    cn_table->setHorizontalHeaderLabels(cn_labels);

    centralLayout->addWidget(tabsWidget);  

    show();
}

MainWindow::~MainWindow()
{}

