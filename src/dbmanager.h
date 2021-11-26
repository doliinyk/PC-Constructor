#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QMainWindow>

namespace Ui {
class DBManager;
}

class DBManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBManager(QWidget *parent = nullptr);
    ~DBManager();

private:
    Ui::DBManager *ui;
};

#endif // DBMANAGER_H
