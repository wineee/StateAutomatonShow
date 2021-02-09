#ifndef MATRIXDLG_H
#define MATRIXDLG_H

#include <QDialog>
#include <QStandardItemModel>
#include "node.h"
#include "graphwidget.h"


namespace Ui {
class MatrixDlg;
}

class MatrixDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixDlg(GraphWidget *gwight);
    explicit MatrixDlg(QVector<Node *> nodelist);

    ~MatrixDlg();

    QStandardItemModel * getModel();
    QStringList * getState();
    void Draw();

private slots:
    void on_pushButton_start_input_f_clicked();
public:
    Ui::MatrixDlg *ui;
    QStandardItemModel *model;
    GraphWidget *wight;
};

#endif // MATRIXDLG_H
