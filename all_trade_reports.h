#ifndef ALL_TRADE_REPORTS_H
#define ALL_TRADE_REPORTS_H

#include <QDialog>
#include <QTextBrowser>

namespace Ui {
class all_trade_reports;
}

class all_trade_reports : public QDialog
{
    Q_OBJECT

public:
    explicit all_trade_reports(QWidget *parent = nullptr);


    ~all_trade_reports();


private slots:
    void on_pushButton_clicked();

private:
    Ui::all_trade_reports *ui;
};

#endif // ALL_TRADE_REPORTS_H
