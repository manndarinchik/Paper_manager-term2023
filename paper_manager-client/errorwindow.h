#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QMainWindow>
#include <QLabel>

class ErrorWindow : public QMainWindow
{
    Q_OBJECT
public:
    ErrorWindow(QWidget *parent = nullptr);
    ~ErrorWindow();

signals:
    void message_shown();
    void message_closed();

public slots:
    void invoke_window(QString message);
private slots:
    void close_message();
private:
    QLabel *error_message;
};
#endif //ERRORWINDOW_H