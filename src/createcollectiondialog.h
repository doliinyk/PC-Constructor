#ifndef CREATECOLLECTIONDIALOG_H
#define CREATECOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateCollectionDialog;
}

class CreateCollectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCollectionDialog(QWidget *parent = nullptr);
    ~CreateCollectionDialog();

signals:
    void getCollectionName(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateCollectionDialog *ui;
};

#endif // CREATECOLLECTIONDIALOG_H
