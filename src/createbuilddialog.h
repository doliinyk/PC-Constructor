#ifndef CREATEBUILDDIALOG_H
#define CREATEBUILDDIALOG_H

#include <QDialog>

namespace Ui {
class CreateBuildDialog;
}

//  Клас-діалогове вікно, яке запитує ім'я для створення збірки
class CreateBuildDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBuildDialog(QWidget *parent = nullptr);
    ~CreateBuildDialog();

signals:
    void getBuildName(QString);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_CreateBuildDialog_finished(int result);

private:
    Ui::CreateBuildDialog *ui;
};

#endif // CREATEBUILDDIALOG_H
