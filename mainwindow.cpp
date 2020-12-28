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

   // connect(ui->comboBox,&QComboBox::currentTextChanged,[=](){::type=ui->type->currentText();});


    connect(ui->exit,SIGNAL(clicked()),SLOT(close()));


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

        ui->checkBox_vol->setChecked(1);


    ::type=ui->type->currentText();

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

    while(!name.atEnd()){

        if(::cat=="بورس ایران"){
         QBarSet *setdata=new QBarSet(name.readLine());
         double profit=((close.readLine().toDouble()/entry.readLine().toDouble())-1)*vol.readLine().toDouble()/1000000;
         *setdata<<profit;

         seri->append(setdata);
        }
        else{
            QBarSet *setdata=new QBarSet(name.readLine());
            double profit=((close.readLine().toDouble()/entry.readLine().toDouble())-1)*vol.readLine().toDouble();
            *setdata<<profit;

            seri->append(setdata);
        }

    }

    QChart *chart = new QChart();
        chart->addSeries(seri);
        if(::cat=="بورس ایران"){
        chart->setTitle("مبالغ به میلیون ریال می باشد.");
        }
        else{
            chart->setTitle("هر واحد ده دلار می باشد.");
        }

      //  connect(seri,&QAbstractBarSeries::clicked,[=](){ seri->QAbstractBarSeries::setLabelsFormat("@value"); seri->setLabelsPosition(QAbstractBarSeries::LabelsCenter);});

       // seri->QAbstractBarSeries::setLabelsFormat("@value");
      //  seri->setLabelsPosition(QAbstractBarSeries::LabelsCenter);

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




       // connect(seri,&QBarSeries::clicked,this,[=](){Text_chart->setText(chart->mapToValue(*val));});


        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(-50,400);
        chart->addAxis(axisY, Qt::AlignLeft);
        seri->attachAxis(axisY);




        chart->legend()->setVisible(true);
             chart->legend()->setAlignment(Qt::AlignBottom);

             QChartView *chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);


                QMainWindow *window=new QMainWindow;
                window->setCentralWidget(chartView);
                window->resize(420, 300);
                window->show();

}

void MainWindow::on_button_name_clicked()
{

   ::name= ui->name->toPlainText();

    write_file name(QDir::currentPath()+"/"+::cat+"/name.txt",::name);

    if(name.isOpen==true){
        ui->checkBox_name->setChecked(1);
    }

}

void MainWindow::on_button_entry_clicked()
{
    ::entry=ui->entry->toPlainText().toDouble();

     write_file entry(QDir::currentPath()+"/"+::cat+"/entry.txt",::entry);

     if(entry.isOpen==true){
         ui->checkBox_entry->setChecked(1);
     }
}

void MainWindow::on_button_close_clicked()
{
     ::close=ui->close->toPlainText().toDouble();
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
     }else{

if(::type=="مقدار ارز"){


   // window->appendText("حجم ریالی معامله : "+QString::number(::vol_trade*entry_num));
    write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",(::vol_trade*entry_num));
    window->appendText("حجم دلاری معامله : "+QString::number(::vol_trade*entry_num));
    window->appendText(" مقدار ارز : " + QString::number(vol_trade));
    window->appendText(" سود درصدی شما از این معامله : "+(QString::number(((close_num / entry_num)-1)*100-0.1)+" % " ));
    window->appendText(" سود دلاری شما از این معامله : "+(QString::number(((close_num / entry_num)-1.001)*::vol_trade*entry_num))+ " $");

}else if(::type=="دلاری"){

 write_file write_vol_trade(QDir::currentPath()+"/"+::cat+"/vol_trade.txt",::vol_trade);
window->appendText("حجم دلاری معامله : "+QString::number(::vol_trade));
window->appendText(" مقدار ارز : " + QString::number(vol_trade /entry_num));
window->appendText(" سود درصدی شما از این معامله : "+(QString::number(((close_num / entry_num)-1)*100-1)+" % " ));
window->appendText(" سود دلاری شما از این معامله : "+(QString::number(((close_num / entry_num)-1.01)*::vol_trade))+ " $");


     }
}


     window->exec();

}



void MainWindow::on_clear_data_button_clicked()
{
   QFile::remove(QDir::currentPath()+"/"+::cat+"/close.txt");
   QFile::remove(QDir::currentPath()+"/"+::cat+"/entry.txt");
   QFile::remove(QDir::currentPath()+"/"+::cat+"/name.txt");
   QFile::remove(QDir::currentPath()+"/"+::cat+"/vol_trade.txt");

}



void MainWindow::on_trade_history_clicked()
{

    all_trade_reports *window=new all_trade_reports;
    window->exec();


}






