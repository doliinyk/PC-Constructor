#ifndef SINGLECOMPONENTWIDGET_H
#define SINGLECOMPONENTWIDGET_H

#include <QWidget>
#include "QSqlTableModel"
#include "sqlitedbmanager.h"

namespace Ui {
class SingleComponentWidget;
}

class SingleComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleComponentWidget(QString, QWidget *parent = nullptr);
    ~SingleComponentWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SingleComponentWidget *ui;
    IDBManager *db;
    QSqlTableModel *model;
};

#endif // SINGLECOMPONENTWIDGET_H
