#include "addedgedlg.h"
#include "ui_addedgedlg.h"

addEdgeDlg::addEdgeDlg(QString nameA, QString nameB, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addEdgeDlg)
{
    ui->setupUi(this);
    ui->lineEdit_stA->setText(nameA);
    ui->lineEdit_edA->setText(nameB);

    ui->lineEdit_stB->setText(nameB);
    ui->lineEdit_edB->setText(nameA);
}

addEdgeDlg::addEdgeDlg(QString nameA, QString nameB, QString inputL, QString reInputL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addEdgeDlg)
{
    ui->setupUi(this);
    ui->lineEdit_stA->setText(nameA);
    ui->lineEdit_edA->setText(nameB);

    ui->lineEdit_stB->setText(nameB);
    ui->lineEdit_edB->setText(nameA);

    ui->lineEdit_inputA->setText(inputL);
    ui->lineEdit_inputB->setText(reInputL);
}

addEdgeDlg::~addEdgeDlg()
{
    delete ui;
}
QString addEdgeDlg::getInputList() {
    return ui->lineEdit_inputA->text();
}

QString addEdgeDlg::getReInputList() {
    return ui->lineEdit_inputB->text();
}
