#include "trade_report.h"
#include "ui_trade_report.h"

trade_report::trade_report(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::trade_report)
{
    ui->setupUi(this);


}

trade_report::~trade_report()
{
    delete ui;
}

void trade_report::appendText(QString x){

    ui->textBrowser->append(x);
}

void trade_report::setFont(int num){

    ui->textBrowser->setFontPointSize(num);
}
