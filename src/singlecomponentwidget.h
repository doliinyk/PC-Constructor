#ifndef SINGLECOMPONENTWIDGET_H
#define SINGLECOMPONENTWIDGET_H

#include <QSqlQuery>
#include <QVector>
#include <QWidget>
#include "sqlitedbmanager.h"

namespace Ui {
class SingleComponentWidget;
}

class SingleComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleComponentWidget(QString componentName,
                                   int buildId,
                                   bool isRestored,
                                   QWidget *parent = nullptr);
    ~SingleComponentWidget();

signals:
    void componentDeleted(QString componentName);

private slots:
    void on_pushButton_clicked();

private:
    Ui::SingleComponentWidget *ui;
    IDBManager *db;

    int buildId;
    int componentId;
    QString componentName;
    QSqlQuery query;

    void componentCompatibility(QString componentName);
    int findIdByName(QString tableName, QString componentName);
    bool compatibilityQuery(QString componentName, QString compareName);
    QString checkSecondComponent(QString componentName);
};

#endif // SINGLECOMPONENTWIDGET_H
