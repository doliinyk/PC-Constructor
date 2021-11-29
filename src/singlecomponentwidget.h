#ifndef SINGLECOMPONENTWIDGET_H
#define SINGLECOMPONENTWIDGET_H

#include <QWidget>

namespace Ui {
class SingleComponentWidget;
}

class SingleComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleComponentWidget(QString, QWidget *parent = nullptr);
    ~SingleComponentWidget();

private:
    Ui::SingleComponentWidget *ui;
};

#endif // SINGLECOMPONENTWIDGET_H
