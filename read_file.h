#ifndef READ_FILE_H
#define READ_FILE_H
#include <QString>
#include <QObject>
#include <QTextBrowser>
class read_file
{

public:

    QString address;

    read_file(QString );
    read_file();
    QString read_name();
    double read_num();

   // void read_all(QTextBrowser ,QString,QString,QString);



};

#endif // READ_FILE_H
