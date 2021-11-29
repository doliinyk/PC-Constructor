#ifndef COMPONENTSMANAGER_H
#define COMPONENTSMANAGER_H

#include <QSqlTableModel>
#include <QTreeWidgetItem>
#include <QWidget>
#include "sqlitedbmanager.h"

namespace Ui {
class ComponentsManager;
}

class ComponentsManager : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentsManager(QWidget *parent = nullptr);
    ~ComponentsManager();

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *);
    void on_tableView_clicked(const QModelIndex &);
    void on_addComponentButton_clicked();
    void on_deleteComponentButton_clicked();

private:
    Ui::ComponentsManager *ui;
    IDBManager *db;
    QSqlTableModel *model;
    unsigned int activeRow;

    QString translateText(QString);
};

#endif // COMPONENTSMANAGER_H
