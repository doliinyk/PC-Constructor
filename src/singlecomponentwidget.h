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
    explicit SingleComponentWidget(QString componentName,
                                   unsigned int buildId,
                                   QWidget *parent = nullptr);
    ~SingleComponentWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SingleComponentWidget *ui;
    IDBManager *db;
    QSqlTableModel *buildModel;
    QSqlTableModel *activeComponentModel;
    QSqlTableModel *compareComponentModel;

    unsigned int buildId;

    bool componentCompatibility(QString componentName);
};

#endif // SINGLECOMPONENTWIDGET_H
