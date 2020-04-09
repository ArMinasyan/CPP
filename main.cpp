#include "login.h"
#include "ui_login.h"
#include <QApplication>


QByteArray data_read(QString &path){
    QFile read(path);
    read.open(QFile::ReadOnly);
    QTextStream read_stream(&read);
    QByteArray res = read.readAll();
    read.close();
    return res;
}

void data_write(QByteArray data, QString path){
    QFile write(path);
    write.open(QFile::WriteOnly);
    write.write(data);
    write.close();
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Login w;
    w.show();

    QFileSystemWatcher watch;
    watch.addPath("Z:/Cloud");
    QObject::connect( &watch, &QFileSystemWatcher::directoryChanged,[]( const QString& path ){
    QDirIterator it( path, QDir::NoDotAndDotDot|QDir::AllEntries);
    //QString file_name;
        while (it.hasNext())
                {
                  QString  jpath=it.next();
                //  qDebug()<<path.mid(9);
                    QByteArray data = data_read(jpath);
                  QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::Padding::ISO);
                  QByteArray hashKey = QCryptographicHash::hash("test", QCryptographicHash::Sha256);
                   QByteArray encodeText = encryption.encode(data, hashKey);
                  // "Z:/Cloud/"+path.mid(0,9).toUtf8().toBase64()
                   data_write(encodeText,jpath);

                   QFile::rename(jpath,"Z:/Cloud/"+jpath.mid(9).toUtf8().toBase64());

//qDebug()<<path.mid(9,path.length()-8);
                }
    });

    return a.exec();

}
