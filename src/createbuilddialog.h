#ifndef CREATEBUILDDIALOG_H
#define CREATEBUILDDIALOG_H

#include <QDialog>

namespace Ui {
class CreateBuildDialog;
}

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

private:
    Ui::CreateBuildDialog *ui;
};

#endif // CREATEBUILDDIALOG_H
