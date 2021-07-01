#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include "write_file.h"
#include "read_file.h"
#include <QMessageBox>
#include "trade_report.h"
#include "ui_trade_report.h"
#include "all_trade_reports.h"
#include <QtCharts>
#include <QAbstractBarSeries>
#include <QPointF>
#include <QGraphicsSimpleTextItem>
#include <QToolTip>
#include <QPixmap>
#include "xlsxdocument.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <windows.h>
#include <QTextStream>
#include <QtAlgorithms>
#include <iostream>









QString name;
QString cat;
QString type;

double entry;
double close;
double vol_trade;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->name,&QTextEdit::textChanged,[=](){ui->checkBox_name->setChecked(0);});
    connect(ui->entry,&QTextEdit::textChanged,[=](){ui->checkBox_entry->setChecked(0);});
    connect(ui->close,&QTextEdit::textChanged,[=](){ui->checkBox_close->setChecked(0);});
    connect(ui->vol_trade,&QTextEdit::textChanged,[=](){ui->checkBox_vol->setChecked(0);});



    connect(ui->comboBox,&QComboBox::currentTextChanged,[=](){ui->type->clear();});


    connect(ui->exit,&QPushButton::clicked,&QMainWindow::close);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_select_cat_clicked()
{
    ::cat=ui->comboBox->currentText();


    if(::cat=="بورس ایران"){

        ui->type->insertItem(1,"تعداد سهم");
        ui->type->insertItem(2,"ریالی");


    }
    else{
        ui->type->insertItem(1,"مقدار ارز");
        ui->type->insertItem(2,"دلاری");
    }
}

void MainWindow::on_vol_trade_button_clicked()
{


    ::vol_trade=ui->vol_trade->toPlainText().toDouble();

    if(::vol_trade==NULL){
        QMessageBox *message=new QMessageBox;
        message->setText("لطفا مقدار را وارد کنید.");
        message->show();

    }else{
        ui->checkBox_vol->setChecked(1);


    ::type=ui->type->currentText();
}
}


void MainWindow::on_graph_clicked()
{

    QBarSeries *seri=new QBarSeries();

    QFile name(QDir::currentPath()+"/"+::cat+"/name.txt");
    QFile entry(QDir::currentPath()+"/"+::cat+"/entry.txt");
    QFile close(QDir::currentPath()+"/"+::cat+"/close.txt");
    QFile vol(QDir::currentPath()+"/"+::cat+"/vol_trade.txt");


    name.open(QFile::ReadOnly);
    entry.open(QFile::ReadOnly);
    close.open(QFile::ReadOnly);
    vol.open(QFile::ReadOnly);


    QVector <double> profit;
    int i=0;
    while(!name.atEnd()){

        if(::cat=="بورس ایران"){

         QBarSet *setdata=new QBarSet(name.readLine());
         profit.push_back(((close.readLine().toDouble()/entry.readLine().toDouble())-1)*vol.readLine().toDouble()/1000000);
         *setdata<<profit[i];
         i++;

         seri->append(setdata);
        }
        else{

            QBarSet *setdata=new QBarSet(name.readLine());
            profit.push_back(((close.readLine().toDouble()/entry.readLine().toDouble())-1)*vol.readLine().toDouble());
            *setdata<<profit[i];
            i++;

            seri->append(setdata);
        }

    }

    std::stable_sort(profit.begin(),profit.end());                                            //sort profits



    QChart *chart = new QChart();
        chart->addSeries(seri);

        if(::cat=="بورس ایران"){
        chart->setTitle("مبالغ به میلیون ریال می باشد.");
        }
        else{
            chart->setTitle("هر واحد یک دلار می باشد.");
        }



        chart->setAnimationOptions(QChart::SeriesAnimations);



        seri->setBarWidth(1);
        seri->setLabelsAngle(3);


        QStringList categories;
        categories << "کل";
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX,Qt::AlignTrailing);
        seri->attachAxis(axisX);


        QPointF *val=new QPointF;

        val->setX(20);

        QGraphicsSimpleTextItem *Text_chart=new QGraphicsSimpleTextItem(chart);
        Text_chart->setPos(*val);



        int floor = 0;
        int index=profit.length()-1;



        while(true){
            if(QString::number(profit[index])!="inf"){
                floor=profit[index];
                break;
            }
            index--;
        }



        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(int(profit[0]*2),(floor*2));
        chart->addAxis(axisY, Qt::AlignLeft);
        seri->attachAxis(axisY);




        chart->legend()->setVisible(true);
             chart->legend()->setAlignment(Qt::AlignBottom);

             QChartView *chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);


                QMainWindow *window=new QMainWindow;
                window->setCentralWidget(chartView);
                window->resize(1000, 600);
                window->show();

}

void MainWindow::on_button_name_clicked()
{

   ::name= ui->name->toPlainText();

    if(::name==NULL){
        QMessageBox *message=new QMessageBox;
        message->setText("لطفا مقدار را وارد کنید.");
        message->show();

    }else{
        write_file name(QDir::currentPath()+"/"+::cat+"/name.txt",::name);

        if(name.isOpen==true){
            ui->checkBox_name->setChecked(1);

        }

    }

}

void MainWindow::on_button_entry_clicked()
{
    ::entry=ui->entry->toPlainText().toDouble();

    if(::entry==NULL){

    QMessageBox *message=new QMessageBox;
    message->setText("لطفا مقدار را وارد کنید.");
    message->show();


}else{

     write_file entry(QDir::currentPath()+"/"+::cat+"/entry.txt",::entry);

     if(entry.isOpen==true){
         ui->checkBox_entry->setChecked(1);

     }
    }

}



void MainWindow::on_button_close_clicked()
{

     ::close=ui->close->toPlainText().toDouble();

    if(::close==NULL){

        QMessageBox *message=new QMessageBox;
        message->setText("لطفا مقدار را وارد کنید.");
        message->show();

    }else{

     ui->checkBox_close->setChecked(1);
     write_file close(QDir::currentPath()+"/"+::cat+"/close.txt",::close);
     read_file readname(QDir::currentPath()+"/"+::cat+"/name.txt");
     read_file readentry(QDir::currentPath()+"/"+::cat+"/entry.txt");
     read_file readclose(QDir::currentPath()+"/"+::cat+"/close.txt");


     trade_report *window=new trade_report;


     double entry_num=readentry.read_num();
     double close_num=readclose.read_num();


     window->trade_report::setFont(13);
     window->appendText("نام : "+readname.read_name());
     window->appendText(" قیمت ورود : " +QString::number(entry_num));
     window->appendText(" قیمت خروج : "+ QString::number(close_num));


     if(::cat=="بورس ایران"){

         if(::type=="تعداد سهم"){

     write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",::vol_trade*entry_num);

     window->appendText("حجم ریالی معامله : "+QString::number(::vol_trade*entry_num));
     window->appendText(" تعداد سهم : " + QString::number(vol_trade));
     window->appendText(" سود درصدی شما از این معامله : "+(QString::number(((close_num / entry_num)-1)*100-1)+" % " ));
     window->appendText(" سود ریالی شما از این معامله : "+(QString::number((((close_num / entry_num)-1.01)*::vol_trade*entry_num)/1000000))+"میلیون ریال");

} else{

         write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",::vol_trade);
         window->appendText("حجم ریالی معامله : "+QString::number(::vol_trade));
         window->appendText(" تعداد سهم : " + QString::number(vol_trade/entry_num));
         window->appendText(" سود درصدی شما از این معامله : "+(QString::number(((close_num / entry_num)-1)*100-1)+" % " ));
         window->appendText(" سود ریالی شما از این معامله : "+(QString::number((((close_num / entry_num)-1.01)*::vol_trade)/1000000) )+ " میلیون ريال");

         }
     }else if(::cat=="کریپتو کارنسی"){

if(::type=="مقدار ارز"){


    write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",(::vol_trade*entry_num));
    window->appendText("حجم دلاری معامله : "+QString::number(::vol_trade*entry_num));
    window->appendText(" مقدار ارز : " + QString::number(vol_trade));
    window->appendText(" درصدی شما از این معامله سود (زیان) : "+(QString::number(((close_num / entry_num)-1)*100-0.1)+" % " ));
    window->appendText(" دلاری شما از این معامله سود (زیان) : "+(QString::number(((close_num / entry_num)-1.001)*::vol_trade*entry_num))+ " $");


}else if(::type=="دلاری"){


write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",::vol_trade);
window->appendText("حجم دلاری معامله : "+QString::number(::vol_trade));
window->appendText(" مقدار ارز : " + QString::number(vol_trade /entry_num));
window->appendText("درصدی شما از این معامله سود (زیان) : "+(QString::number(((close_num / entry_num)-1)*100-0.1)+" % " ));
window->appendText("دلاری شما از این معامله سود (زیان) : "+(QString::number(((close_num / entry_num)-1.01)*::vol_trade))+ " $");


     }
}


     window->exec();

}

}



void MainWindow::on_clear_data_button_clicked()
{

  bool close= QFile::remove(QDir::currentPath()+"/"+::cat+"/close.txt");
  bool entry=QFile::remove(QDir::currentPath()+"/"+::cat+"/entry.txt");
  bool name=QFile::remove(QDir::currentPath()+"/"+::cat+"/name.txt");
  bool vol=QFile::remove(QDir::currentPath()+"/"+::cat+"/vol_trade.txt");

  if(close==true&&entry==true&&name==true&&vol==true){

      QMessageBox * message=new QMessageBox;
      message->setText("حذف داده با موفقیت انجام شد!");
      message->show();

 }else{

      QMessageBox * message=new QMessageBox;
      message->setText("حذف داده ناموفق بود!");
      message->show();

  }

}




void MainWindow::on_trade_history_clicked()
{

    all_trade_reports *window=new all_trade_reports;
    window->exec();

}



void MainWindow::on_import_excel_clicked()
{


    QFileDialog *file_dialog = nullptr;
    QString excel_add=file_dialog->getOpenFileName(this,tr("فایل اکسل را انتخاب کنید."),tr("فایل اکسل"),"Text (*.xlsx)");

    QFile write_name(QDir::currentPath()+"/کریپتو کارنسی/name.txt");
    QFile write_vol(QDir::currentPath()+"/کریپتو کارنسی/vol_trade.txt");
    QFile write_entry(QDir::currentPath()+"/کریپتو کارنسی/entry.txt");
    QFile write_close(QDir::currentPath()+"/کریپتو کارنسی/close.txt");


    QTextStream swrite_name(&write_name);
    QTextStream swrite_vol(&write_vol);
    QTextStream swrite_entry(&write_entry);
    QTextStream swrite_close(&write_close);


    write_name.open(QFile::WriteOnly|QFile::Append|QFile::Text);
    write_vol.open(QFile::WriteOnly|QFile::Append|QFile::Text);
    write_entry.open(QFile::WriteOnly|QFile::Append|QFile::Text);
    write_close.open(QFile::WriteOnly|QFile::Append|QFile::Text);



    QVBoxLayout *lay=new QVBoxLayout;
    QWidget *progress_wid=new QWidget;
    QProgressBar *bar=new QProgressBar;
    QLabel *label=new QLabel;

    label->setText("در حال بارگذاری...");

    lay->addWidget(bar);
    lay->addWidget(label);
    progress_wid->setLayout(lay);




    QXlsx::Document *doc=new QXlsx::Document(excel_add);



    int RawI=2;
    int bar_val=10;

    if(doc->load()){

        bar->setValue(bar_val);
        progress_wid->show();


    while(doc->read("B"+QString::number(RawI)).toString()!=NULL){

        bar->setValue(bar_val++);

       QString status= doc->read("I"+QString::number(RawI)).toString();


       int RawB=2;

       if(status=="Filled"){


           QString symbol= doc->read("B"+QString::number(RawI)).toString();


           QVector <double> entry;
           QVector <double> close;
           QVector <double> amount_buy;
           QVector <double> amount_sell;
           QVector <double> total;


           while(doc->read("B"+QString::number(RawB)).toString()!=NULL){


               if(doc->read("I"+QString::number(RawB)).toString()=="Filled"){


               if(symbol==doc->read("B"+QString::number(RawB))){


               if(doc->read("C"+QString::number(RawB)).toString()=="BUY"){


                   entry.push_back(doc->read("F"+QString::number(RawB)).toDouble());
                   amount_buy.push_back(doc->read("G"+QString::number(RawB)).toDouble());


               }else if (doc->read("C"+QString::number(RawB)).toString()=="SELL"){

                   close.push_back(doc->read("F"+QString::number(RawB)).toDouble());
                   amount_sell.push_back(doc->read("H"+QString::number(RawB)).toDouble());
                   total.push_back(doc->read("H"+QString::number(RawB)).toDouble());

               }

               doc->write("I"+QString::number(RawB),"NULL");

               }

               }

               RawB++;


           }


           double avg_buy=0;
           double avg_sell=0;
           double all_buy=0;
           double all_sell=0;
           double all_totals=0;



              for(int i=0;i<entry.length();i++){

              avg_buy+=entry[i]*amount_buy[i];
              all_buy+=amount_buy[i];

              }

              for(int i=0;i<close.length();i++){

              avg_sell+=close[i]*amount_sell[i];
              all_sell+=amount_sell[i];

              }

              for(int i=0;i<total.length();i++){

              all_totals+=total[i];

              }

              if(symbol.contains("BTCUSDT")){

                  write_entry.write(QString::number(avg_buy/all_buy).toUtf8());
                  swrite_entry<<Qt::endl;

                  write_close.write(QString::number(avg_sell/all_sell).toUtf8());
                  swrite_close<<Qt::endl;

                  write_name.write(symbol.toUtf8());
                  swrite_name<<Qt::endl;

                  write_vol.write(QString::number(all_totals).toUtf8());
                  swrite_vol<<Qt::endl;

              }else if(symbol.contains("USDT")){

           write_entry.write(QString::number(avg_buy/all_buy).toUtf8());
           swrite_entry<<Qt::endl;

           write_close.write(QString::number(avg_sell/all_sell).toUtf8());
           swrite_close<<Qt::endl;

           write_name.write(symbol.toUtf8());
           swrite_name<<Qt::endl;

           write_vol.write(QString::number(all_totals).toUtf8());
           swrite_vol<<Qt::endl;

             }
             else if(symbol.contains("BTC")){

                 QEventLoop loop;
                 QNetworkAccessManager *acc=new QNetworkAccessManager;
                 QNetworkRequest req(QUrl("https://api3.binance.com/api/v3/ticker/price?symbol=BTCUSDT"));

                 QNetworkReply *rep=acc->get(req);

                 connect(rep,&QNetworkReply::finished,&loop,&QEventLoop::quit);

                 loop.exec();


                 QJsonDocument doc=QJsonDocument::fromJson(rep->readAll());
                 QJsonObject obj=doc.object();

                double price=obj.value("price").toString().toDouble();


                 if(price!=NULL){

                 write_entry.write(QString::number(avg_buy/all_buy).toUtf8());
                 swrite_entry<<Qt::endl;

                 write_close.write(QString::number(avg_sell/all_sell).toUtf8());
                 swrite_close<<Qt::endl;

                 write_name.write(symbol.toUtf8());
                 swrite_name<<Qt::endl;


                 write_vol.write(QString::number(all_totals*price).toUtf8());
                 swrite_vol<<Qt::endl;

                 }else{
                     QMessageBox * message=new QMessageBox;
                     message->setText("از اتصال به VPN اطمینان حاصل کرده و مجددا سعی کنید!");
                     message->show();

                     break;
                 }

             }

       }

       RawI++;

    }

    }


    progress_wid->close();


}


                                                                          //END





