#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include "matrixdlg.h"
#include "node.h"
#include "edge.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gwidget = new GraphWidget;
    this->setCentralWidget(gwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_move_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_MOVE);
}

void MainWindow::on_radioButton_addNode_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_ADDNODE);
}

void MainWindow::on_radioButton_addEdge_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_ADDEDGE1);
}

void MainWindow::on_radioButton_delete_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_DELETE);
}

void MainWindow::on_radioButton_modify_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_MODIFY);
}

void MainWindow::on_checkBox_abjust_toggled(bool checked)
{
    gwidget->setautoAdjust(checked);
}

// 保存为文件
void MainWindow::on_pushButton_toFile_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("保存文件"),"/home/rewine/文档/");
    qDebug()<<filePath;
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) {
           QMessageBox::warning(this, tr("打开文件错误"),"");
    }
    QTextStream stream(&file);

    auto nodeList = Node::getNodeList();
    stream << nodeList.size() << "\n";
    for (Node *node : nodeList) {
       stream << node->getName() << " " << node->getCas() << " "
              << node->pos().x() << " " << node->pos().y() << "\n";
    }
    int cntEdge = 0;
    for (Node *node : nodeList) {
        for (const Edge *edge : node->edges()) {
            if (edge->sourceNode() == node) cntEdge++;
        }
    }
    stream << cntEdge << "\n";
    for (Node *node : nodeList) {
        for (const Edge *edge : node->edges()) {
            if (edge->sourceNode() == node) {
                stream << edge->sourceNode()->getName() << " " << edge->destNode()->getName() << " "
                       << edge->getInputList() << " #" << edge->getReInputList() << "\n";
            }
        }
    }
    file.close();
}

// 从文件导入
void MainWindow::on_pushButton_fromFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"),"/home/rewine/文档/");
    qDebug()<<filePath;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开文件错误"), "");
    }
    QTextStream stream(&file);
    int cntNode = 0, cntEdge = 0;
    stream >> cntNode;
    for (int i = 0; i < cntNode; i++) {
        QString name; int Cas; qreal x; qreal y;
        stream >> name >> Cas >> x >> y;
        gwidget->addNode(name, Cas, x, y);
        qDebug() << name << " " << Cas << " " << x << " " << y <<"\n";
    }

    stream >> cntEdge;
    for (int i = 0; i < cntEdge; i++) {
        QString A, B, input, reInput;
        stream >> A >> B >> input >> reInput;
        qDebug() << A << " " << B << " " << input << " " << reInput.remove('#') <<"\n";
        gwidget->addEdge(A, B, input, reInput.remove('#'));
    }
    file.close();
}

// 变矩阵
void MainWindow::on_pushButton_toMatrix_clicked()
{
    MatrixDlg dlg(Node::getNodeList());
    dlg.exec();
}

// 由矩阵导入
void MainWindow::on_pushButton_fromMatrix_clicked()
{
    MatrixDlg dlg(gwidget);
    if (dlg.exec() == QDialog::Accepted) {
       dlg.Draw();
    }
}
