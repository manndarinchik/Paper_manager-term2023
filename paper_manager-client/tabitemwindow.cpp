#include "tabitemwindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTableView>

AddPublicationWindow::AddPublicationWindow(PSQLInterface* psqli, QWidget *parent)
    : BaseWindow(psqli, "Новая публикация", parent){}

void AddPublicationWindow::init()
{
    QFormLayout *form = new QFormLayout();
    QHBoxLayout *tablesL = new QHBoxLayout();
    QVBoxLayout *authorsL = new QVBoxLayout();
    QVBoxLayout *compilationsL = new QVBoxLayout();
    QHBoxLayout *btnL = new QHBoxLayout();
    tablesL->addLayout(authorsL);
    tablesL->addLayout(compilationsL);
    centralL->addLayout(form);
    centralL->addLayout(tablesL);
    centralL->addLayout(btnL);
    centralL->setAlignment(btnL, Qt::AlignCenter);
    
    QPushButton *addAuthor = new QPushButton("Добавить автора");
    QPushButton *addCompilation = new QPushButton("Добавить Сборник");

    QLineEdit *newName = new QLineEdit();
    QComboBox *newType = new QComboBox();
    QDateEdit *newDate = new QDateEdit();
    QLabel *newPublisherDisplay = new QLabel();
    newPublisherDisplay->setStyleSheet("border: 1px solid black;");
    newPublisherDisplay->setCursor(Qt::PointingHandCursor);
    unsigned int newPublisher;
    QTableView *newAuthorsDisplay = new QTableView();
    std::vector<unsigned int> newAuthors = {};
    QTableView *newCompilationsDisplay = new QTableView();
    std::vector<unsigned int> newCompilations = {};

    QPushButton *finishBtn = new QPushButton("Создать");
    finishBtn->setStyleSheet("background-color: #5eba7d;");
    finishBtn->setMaximumWidth(100);
    QPushButton *abortBtn = new QPushButton("Отменить");
    abortBtn->setStyleSheet("background-color: #ba5e5e;");
    abortBtn->setMaximumWidth(100);

    form->addRow("Название", newName);
    form->addRow("Тип", newType);
    form->addRow("Дата", newDate);
    form->addRow("Издательство", newPublisherDisplay);
    authorsL->addWidget(addAuthor);
    authorsL->addWidget(newAuthorsDisplay);
    compilationsL->addWidget(addCompilation);
    compilationsL->addWidget(newCompilationsDisplay);
    btnL->addWidget(finishBtn);
    btnL->addWidget(abortBtn);

    show();
}