#include "all_trade_reports.h"
#include "ui_all_trade_reports.h"
#include "read_file.h"
#include "mainwindow.h"
#include <QDir>
#include <QFile>

all_trade_reports::all_trade_reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::all_trade_reports)
{
    ui->setupUi(this);
    ui->textBrowser->setFontPointSize(13);

}


all_trade_reports::~all_trade_reports()
{
    delete ui;
}

void all_trade_reports::on_pushButton_clicked()
{

    QString cat=ui->comboBox->currentText();

    QFile file_name(QDir::currentPath()+"/"+cat+"/name.txt");
    QFile file_entry(QDir::currentPath()+"/"+cat+"/entry.txt");
    QFile file_close(QDir::currentPath()+"/"+cat+"/close.txt");
    QFile file_vol(QDir::currentPath()+"/"+cat+"/vol_trade.txt");


    file_name.open(QFile::ReadOnly);
    file_entry.open(QFile::ReadOnly);
    file_close.open(QFile::ReadOnly);
    file_vol.open(QFile::ReadOnly);



    while(!file_name.atEnd()){

        QString entry=file_entry.readLine();
        QString close=file_close.readLine();

        double vol=file_vol.readLine().toDouble();

       ui->textBrowser->append(" نام : " +file_name.readLine());
       ui->textBrowser->append("قیمت ورود : "+entry);
       ui->textBrowser->append("قیمت خروج : "+close);
       ui->textBrowser->append("حجم معامله : "+QString::number(vol));

       if(cat=="بورس ایران"){
       ui->textBrowser->append("سود درصدی شما از این معامله : "+QString::number((close.toDouble()/entry.toDouble()-1.01)*100)+" %");
       ui->textBrowser->append(" سود ریالی شما از این معامله : "+QString::number((close.toDouble()/entry.toDouble()-1.01)*vol));
       }
       else{
           ui->textBrowser->append("سود درصدی شما از این معامله : "+QString::number((close.toDouble()/entry.toDouble()-1.001)*100)+" %");
           ui->textBrowser->append(" سود دلاری شما از این معامله : "+QString::number((close.toDouble()/entry.toDouble()-1.001)*vol)+"$");
       }
       ui->textBrowser->append("---------------------------------------------------------------------------------------------------------------------------------------------");
    }

    file_entry.close();
    file_name.close();
    file_close.close();
    file_vol.close();

}
