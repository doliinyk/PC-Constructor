#ifndef SPECIFICATIONSWIDGET_H
#define SPECIFICATIONSWIDGET_H

#include <QWidget>

namespace Ui {
class SpecificationsWidget;
}

class SpecificationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpecificationsWidget(QWidget *parent = nullptr);
    ~SpecificationsWidget();

private:
    Ui::SpecificationsWidget *ui;
};

#endif // SPECIFICATIONSWIDGET_H
