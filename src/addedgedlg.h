#ifndef ADDEDGEDLG_H
#define ADDEDGEDLG_H

#include <QDialog>

namespace Ui {
class addEdgeDlg;
}

class addEdgeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit addEdgeDlg(QString nameA, QString nameB, QWidget *parent = nullptr);
    explicit addEdgeDlg(QString nameA, QString nameB, QString inputL, QString reInputL, QWidget *parent = nullptr);
    ~addEdgeDlg();

    QString getInputList();
    QString getReInputList();
private:
    Ui::addEdgeDlg *ui;
};

#endif // ADDEDGEDLG_H
