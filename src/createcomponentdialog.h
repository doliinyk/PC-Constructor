#ifndef CREATECOMPONENTDIALOG_H
#define CREATECOMPONENTDIALOG_H

#include <QDialog>

namespace Ui {
class CreateComponentDialog;
}

class CreateComponentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateComponentDialog(QWidget *parent = nullptr);
    ~CreateComponentDialog();

private:
    Ui::CreateComponentDialog *ui;
};

#endif // CREATECOMPONENTDIALOG_H
