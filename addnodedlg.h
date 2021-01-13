#ifndef ADDNODEDLG_H
#define ADDNODEDLG_H

#include <QDialog>

namespace Ui {
class addNodeDlg;
}

class addNodeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit addNodeDlg(QWidget *parent = nullptr);
    explicit addNodeDlg(QString name, int nodeCas, QWidget *parent = nullptr);

    ~addNodeDlg();

    QString getName();
    int getCas();
private:
    Ui::addNodeDlg *ui;
};

#endif // ADDNODEDLG_H
