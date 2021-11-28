#ifndef DBMANAGERWINDOW_H
#define DBMANAGERWINDOW_H

#include <QMainWindow>

namespace Ui {
class DBManagerWindow;
}

class DBManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBManagerWindow(QWidget *parent = nullptr);
    ~DBManagerWindow();

private:
    Ui::DBManagerWindow *ui;
};

#endif // DBMANAGERWINDOW_H
