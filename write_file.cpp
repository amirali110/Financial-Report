#include "write_file.h"
#include <QString>
#include <QFile>
#include <QTextStream>




write_file::write_file(QString add,QString name)
{

    QFile file(add);
    file.open(QFile::WriteOnly|QFile::Append|QFile::Text);
    isOpen=file.isOpen();

    QTextStream out(&file);
    file.write(name.toUtf8());
    out<<Qt::endl;
    file.close();

}
write_file::write_file(QString add,double num){
    QFile file(add);
    file.open(QFile::WriteOnly|QFile::Append|QFile::Text);
    isOpen=file.isOpen();

    file.write(QString::number(num).toUtf8());
    QTextStream out(&file);
    out<<Qt::endl;
     file.close();

}



