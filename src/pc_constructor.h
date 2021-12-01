#ifndef PC_CONSTRUCTOR_H
#define PC_CONSTRUCTOR_H

#include <QMainWindow>
#include <QTreeWidget>
#include "componentsmanager.h"
#include "componentswidget.h"
#include "createbuilddialog.h"
#include "specificationswidget.h"
#include "sqlitedbmanager.h"

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
    void on_action_NewBuild_triggered();
    void createBuild(QString buildName);
    void on_actionDeleteBuild_triggered();
    void on_tabWidget_tabBarClicked(int index);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *build);
    void on_actionOpenComponentsManager_triggered();
    void on_actionAbout_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::PC_Constructor *ui;
    IDBManager *db;

    QString activeBuildName;
    CreateBuildDialog createBuildDialog;
    ComponentsManager componentsManager;

    void setValidBuildMenuActions();
    void setTreeWidgetBuilds();
    void setTabWidgetBuilds();
};

#endif // PC_CONSTRUCTOR_H
