#include "matrixdlg.h"
#include "ui_matrixdlg.h"
#include "QMessageBox"
#include "node.h"
#include "edge.h"
#include <QStringList>
#include <QDebug>
#include <QRandomGenerator>

MatrixDlg::MatrixDlg(GraphWidget *gwight) :
    QDialog(nullptr),
    ui(new Ui::MatrixDlg)
{
    ui->setupUi(this);
    model = new QStandardItemModel;
    ui->tableView->setModel(model);
    wight = gwight;
}


MatrixDlg::MatrixDlg(QVector<Node *> nodelist):
    QDialog(nullptr),
    ui(new Ui::MatrixDlg)
{
    ui->setupUi(this);
    QString s_state_set;
   // QString s_input_list;
    QStringList sl_input_list;
    QString s_init_state;
    QString s_final_state_set;
    for (Node * node : nodelist) {
        s_state_set += (s_state_set.isEmpty()?"":",") + node->getName();
        if (node->getCas() & 1) {
            s_init_state = node->getName();
        }
        if (node->getCas() & 2) {
            s_final_state_set += (s_final_state_set.isEmpty()?"":",") + node->getName();
        }

        for (const Edge *edge : node->edges()) {
            if (edge->sourceNode() == node) {
                QStringList inputlist = edge->getInputList().split(',');
                for (int i = 0; i < inputlist.size(); i++) {
                   if(!sl_input_list.contains(inputlist[i]))
                       sl_input_list.append(inputlist[i]);
                }
            }
        }
    }
    model = new QStandardItemModel;
    ui->tableView->setModel(model);

    model->setColumnCount(sl_input_list.size());
    for (int i = 0; i < sl_input_list.size(); i++)
        model->setHeaderData(i,Qt::Horizontal, sl_input_list[i]);

    QStringList sl_state_set = s_state_set.split(",");
    model->setRowCount(sl_state_set.size());
    for (int i = 0; i < sl_state_set.size(); i++)
        model->setHeaderData(i, Qt::Vertical, sl_state_set[i]);

    for (Node * node : nodelist) {
        for (const Edge *edge : node->edges()) {
            if (edge->sourceNode() == node) {
                int idRow = 0;
                for (int i = 0; i < sl_state_set.size(); i++)
                    if (sl_state_set[i] == edge->sourceNode()->getName())
                        idRow = i;
                for (int idCol = 0; idCol < sl_input_list.size(); idCol++)
                    if (edge->getInputList().split(",").contains(sl_input_list[idCol]))
                        model->setItem(idRow, idCol, new QStandardItem(edge->destNode()->getName()));
                if (!edge->getReInputList().isEmpty()) {  //
                    int idRow = 0;
                    for (int i = 0; i < sl_state_set.size(); i++)
                        if (sl_state_set[i] == edge->destNode()->getName())
                            idRow = i;
                    for (int idCol = 0; idCol < sl_input_list.size(); idCol++)
                        if (edge->getReInputList().split(",").contains(sl_input_list[idCol]))
                            model->setItem(idRow, idCol, new QStandardItem(edge->sourceNode()->getName()));
                }
            }
        }
    }

    ui->lineEdit_state_set->setText(s_state_set);
    ui->lineEdit_init_state->setText(s_init_state);
    ui->lineEdit_final_state_set->setText(s_final_state_set);
    ui->lineEdit_input_set->setText(sl_input_list.join(","));
}


MatrixDlg::~MatrixDlg()
{
    delete ui;
    delete model;
}

QStandardItemModel *MatrixDlg::getModel() {
    return model;
}


void MatrixDlg::on_pushButton_start_input_f_clicked()
{
    QString s_state_set = ui->lineEdit_state_set->text();
    QString s_input_list = ui->lineEdit_input_set->text();
    QString s_init_state = ui->lineEdit_init_state->text();
    QString s_final_state_set = ui->lineEdit_final_state_set->text();
    if (s_state_set.isEmpty() || s_input_list.isEmpty() || s_init_state.isEmpty() || s_final_state_set.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("输入不能有空值!"));
    }
    QStringList sl_state_set = s_state_set.split(',');
    QStringList sl_input_list = s_input_list.split(',');
    QStringList sl_final_state_set = s_final_state_set.split(',');
     /*设置列字段名*/
    model->setColumnCount(sl_input_list.size());
    for (int i = 0; i < sl_input_list.size(); i++)
        model->setHeaderData(i,Qt::Horizontal, sl_input_list[i]);
     /*设置行字段名*/
    model->setRowCount(sl_state_set.size());
    for (int i = 0; i < sl_state_set.size(); i++)
        model->setHeaderData(i, Qt::Vertical, sl_state_set[i]);
}

void MatrixDlg::Draw() {
    QString s_state_set = ui->lineEdit_state_set->text();
    QString s_input_list = ui->lineEdit_input_set->text();
    QString s_init_state = ui->lineEdit_init_state->text();
    QString s_final_state_set = ui->lineEdit_final_state_set->text();

    QStringList sl_state_set = s_state_set.split(',');
    QStringList sl_input_list = s_input_list.split(',');
    QStringList sl_final_state_set = s_final_state_set.split(',');

    for (int i = 0; i < sl_state_set.size(); i++) {
        int Cas = 0;
        if (s_init_state == sl_state_set[i])
            Cas |= 1;
        if (sl_final_state_set.contains(sl_state_set[i]))
            Cas |= 2;
        wight->addNode(sl_state_set[i], Cas, QRandomGenerator::global()->bounded(250), QRandomGenerator::global()->bounded(250));
    }
    for (int i = 0; i < sl_state_set.size(); i++)
       for (int j = 0; j <= i; j++) {  //求两点间的边
          QString input = "";
          QString reInput = "";
          for (int k = 0; k < sl_input_list.size(); k++) {
              if (!model->item(i, k)) continue;
              qDebug() << i << " " << j << " " << k << " " << model->item(i, k)->text();
              if (model->item(i, k)->text().split(",").contains(sl_state_set[j]))
                  input += (input.isEmpty()?"":",") + sl_input_list[k];
          }
          qDebug() << i << " " << j << " " << input;

          if(i != j)
            for (int k = 0; k < sl_input_list.size(); k++) {
                if (!model->item(j, k)) continue;
              if (model->item(j, k)->text().split(",").contains(sl_state_set[i]))
                  reInput += (reInput.isEmpty()?"":",") + sl_input_list[k];
            }
          if (!input.isEmpty())
              wight->addEdge(sl_state_set[i], sl_state_set[j], input, reInput);
          if (input.isEmpty() && !reInput.isEmpty())
              wight->addEdge(sl_state_set[j], sl_state_set[i], reInput, input);

          qDebug() << sl_state_set[i] << " " << sl_state_set[j] << " " << input << " "<<reInput;

       }
}
