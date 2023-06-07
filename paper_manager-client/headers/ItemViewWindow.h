#ifndef ITEMVIEWWINDOW_H
#define ITEMVIEWWINDOW_H
#include "BaseWindow.h"
#include "PSQLinterface.h"
#include <QComboBox>
#include <QWidget>
#include <QDateEdit>
#include <QSqlQueryModel>
#include <QTableView>
#include <QModelIndex>
#include <QMenu>

class ListTableView : public QTableView{
    Q_OBJECT
private:
    QString base_query, filter_field;
    PSQLInterface* db;
    QMenu *menu;
public:
    std::vector<unsigned int>* list;
    ListTableView(PSQLInterface *psqli, QWidget *parent, QString base_query, QString filder_field, std::vector<unsigned int>* list);
    void update_display();
    QString exclusion_query();

private slots:
    void show_context_menu(QPoint pos);
};

class ItemViewWindow : public BaseWindow
{
    Q_OBJECT
protected:
    int itemID;

public:
    ItemViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1);
    ~ItemViewWindow(){};
    static QString insert_array_into_query(QString base_query, std::vector<unsigned int>* list, QString filter_tail);
    static QString format_date(QDate d);

protected:
    virtual void populate_with_db_data(){};
    void add_table_item(ListTableView* table);
    void select_from_query(QPushButton* display, int* id, QString base_query, QString filter_field);
    QHBoxLayout* create_edit_buttons();
signals:
    void edited_id(int id);

protected slots:
    virtual void submit(){};
    virtual void remove_item(){};
};

class PublicationViewWindow : public ItemViewWindow
{
    Q_OBJECT
private:
    QLineEdit *newName;
    QComboBox *newType;
    QDateEdit *newDate;
    ListTableView *newAuthorsDisplay;
    ListTableView *newCompilationsDisplay;
    int newPublisher;
    QPushButton *newPublisherDisplay;
    std::vector<unsigned int> newAuthors;
    std::vector<unsigned int> newCompilations;

public:
    PublicationViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1);
    ~PublicationViewWindow(){};
private:
    void populate_with_db_data() override;
protected slots:
    void submit() override;
    void remove_item() override;
};

class AuthorViewWindow : public ItemViewWindow
{
    Q_OBJECT
private:
    QLineEdit *newName;
    QLineEdit *newDegree;
    ListTableView *newPublicationsView;
    std::vector<unsigned int> newPublications;

public:
    AuthorViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1);
    ~AuthorViewWindow(){}
private:
    void populate_with_db_data() override;
protected slots:
    void submit() override;
    void remove_item() override;
};

// class PublisherViewWindow : public ItemViewWindow
// {
//     Q_OBJECT
// private:
//     QLineEdit *newName;
//     QLineEdit *newCountry;
//     QLineEdit *newCity;
//     QLineEdit *newAddress;
//     QLineEdit *newNum;
//     QLineEdit *newEmail;
// public:
//     PublisherViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1);
//     ~PublisherViewWindow(){}
// private:
//     void populate_with_db_data() override;
// protected slots:
//     void submit() override;
// };


// class CompilationViewWindow : public ItemViewWindow
// {
//     Q_OBJECT
// private:
//     QLineEdit *newName;
//     QDate *newDate;
//     int *newPublisher;
//     ListTableView *newPublications;

// public:
//     CompilationViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1);
//     ~CompilationViewWindow(){}
// private:
//     void populate_with_db_data() override;
// protected slots:
//     void submit() override;
// };

#endif //ITEMVIEWWINDOW_H