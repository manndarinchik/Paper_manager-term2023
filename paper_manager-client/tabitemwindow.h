#ifndef TABITEMWINDOW_H
#define TABITEMWINDOW_H
#include "basewindow.h"

class AddPublicationWindow : public BaseWindow
{
    Q_OBJECT

public:
    AddPublicationWindow(PSQLInterface* psqli, QWidget *parent);

public slots:
    void init();

private:

};

#endif //TABITEMWINDOW_H