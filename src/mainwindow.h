#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GraphWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_move_toggled(bool checked);

    void on_radioButton_addNode_toggled(bool checked);

    void on_radioButton_addEdge_toggled(bool checked);

    void on_radioButton_delete_toggled(bool checked);

    void on_radioButton_modify_toggled(bool checked);

    void on_checkBox_abjust_toggled(bool checked);

    void on_pushButton_toFile_clicked();

    void on_pushButton_fromFile_clicked();

    void on_pushButton_toMatrix_clicked();

    void on_pushButton_fromMatrix_clicked();

private:
    Ui::MainWindow *ui;
    GraphWidget *gwidget;
};
#endif // MAINWINDOW_H
