#include "read_file.h"
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QWidget>
#include <QTextBrowser>
#include <QDialog>
#include "ui_all_trade_reports.h"

read_file::read_file(QString add)
{
    address=add;
}
read_file::read_file(){

}

QString read_file::read_name(){

    QFile file(address);
    file.open(QFile::ReadOnly);

    int i=0;
    int n=0;

       while(!file.atEnd()){        //find numbers of line in file
           file.readLine();
           i++;
       }

       file.QFile::seek(0);

       while(!file.atEnd()){

           n++;
           QString line=file.readLine();

           if(n==i){
               return line;
           }
       }

       file.close();

}

double read_file::read_num(){


    QFile file(address);
    file.open(QFile::ReadOnly);

    int i=0;
    int n=0;

       while(!file.atEnd()){        //find numbers of line in file
           file.readLine();
           i++;
       }

       file.QFile::seek(0);

       while(!file.atEnd()){

           n++;
           double line=file.readLine().toDouble();

           if(n==i){
               return line;
           }
       }

       file.close();




}

/*
void read_file::read_all(QTextBrowser x,QString addname,QString addentry,QString addclose){

    QFile file_name(addname);
    QFile file_entry(addentry);
    QFile file_close(addclose);


    file_name.open(QFile::ReadOnly);
    file_entry.open(QFile::ReadOnly);
    file_close.open(QFile::ReadOnly);



    while(!file_name.atEnd()){

        QString entry=file_entry.readLine();
        QString close=file_close.readLine();

       x.QTextBrowser::append("نام " +file_name.readLine());
       x.QTextBrowser::append("قیمت ورود : "+entry);
       x.QTextBrowser::append("قیمت خروج : "+close);
       x.QTextBrowser::append("سود درصدی شما از این معامله : "+QString::number((close.toDouble()/entry.toDouble()-1.01)*100));
       x.QTextBrowser::append("--------------------------------");
    }

    file_entry.close();
    file_name.close();
    file_close.close();



}
*/






