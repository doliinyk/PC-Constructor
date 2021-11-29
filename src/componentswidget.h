#ifndef COMPONENTSWIDGET_H
#define COMPONENTSWIDGET_H

#include <QWidget>

namespace Ui {
class ComponentsWidget;
}

class ComponentsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentsWidget(QWidget *parent = nullptr);
    ~ComponentsWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ComponentsWidget *ui;
    QVector<QString> componentNames;
};

#endif // COMPONENTSWIDGET_H
