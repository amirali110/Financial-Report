#ifndef TRADE_REPORT_H
#define TRADE_REPORT_H

#include <QDialog>


namespace Ui {
class trade_report;
}

class trade_report : public QDialog
{
    Q_OBJECT

public:
    explicit trade_report(QWidget *parent = nullptr);

    void appendText(QString);
    void setFont(int );

    ~trade_report();

private:
    Ui::trade_report *ui;
};

#endif // TRADE_REPORT_H
