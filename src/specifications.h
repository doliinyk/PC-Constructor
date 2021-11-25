#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

#include <QWidget>

namespace Ui {
class Specifications;
}

class Specifications : public QWidget
{
    Q_OBJECT

public:
    explicit Specifications(QWidget *parent = nullptr);
    ~Specifications();

private:
    Ui::Specifications *ui;
};

#endif // SPECIFICATIONS_H
