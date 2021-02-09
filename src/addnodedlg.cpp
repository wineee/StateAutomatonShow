#include "addnodedlg.h"
#include "ui_addnodedlg.h"

addNodeDlg::addNodeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addNodeDlg)
{
    ui->setupUi(this);
}

addNodeDlg::addNodeDlg(QString name, int nodeCas, QWidget *parent) :
     QDialog(parent),
     ui(new Ui::addNodeDlg)
{
    ui->setupUi(this);
    ui->lineEdit_name->setText(name);
    if (nodeCas & 1)
        ui->checkBox_initialState->setChecked(true);
    if (nodeCas & 2)
        ui->checkBox_finalState->setChecked(true);
}


addNodeDlg::~addNodeDlg()
{
    delete ui;
}

QString addNodeDlg::getName() {
    return ui->lineEdit_name->text();
}

int addNodeDlg::getCas() {
    int cas = 0;
    if (ui->checkBox_initialState->isChecked())
        cas |= 1;
    if (ui->checkBox_finalState->isChecked())
        cas |= 2;
    return cas;
}
