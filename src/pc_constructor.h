#ifndef PC_CONSTRUCTOR_H
#define PC_CONSTRUCTOR_H

#include <QLabel>
#include <QMainWindow>

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

private:
    Ui::PC_Constructor *ui;
};

#endif // PC_CONSTRUCTOR_H
