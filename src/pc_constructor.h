#ifndef PC_CONSTRUCTOR_H
#define PC_CONSTRUCTOR_H

#include <QLabel>
#include <QMainWindow>
#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PC_Constructor;
}
QT_END_NAMESPACE

class PC_Constructor : public QMainWindow
{
    Q_OBJECT

public:
    PC_Constructor(QWidget *parent = nullptr);
    ~PC_Constructor();

private slots:
    void on_action_triggered();

private:
    Ui::PC_Constructor *ui;
    DBManager dbmanager;
};

#endif // PC_CONSTRUCTOR_H
