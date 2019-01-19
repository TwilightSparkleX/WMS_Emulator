#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    abs=ui->textEdit->toPlainText();
    port=ui->portedit->toPlainText().toInt();
    emit buttonPressed1(abs,port);

}
