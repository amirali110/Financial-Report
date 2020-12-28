#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();




private slots:
    void on_graph_clicked();

    void on_button_name_clicked();

    void on_button_entry_clicked();

    void on_button_close_clicked();

    void on_select_cat_clicked();

    void on_clear_data_button_clicked();

    void on_vol_trade_button_clicked();

    void on_trade_history_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
