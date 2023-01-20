#include <QCoreApplication>
#include <QFile>
#include <QDebug>

#include "generator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QString _str;
//    QFile _setPort("../Generator/setPort.conf");
//    if ( (_setPort.exists()) && (_setPort.open(QIODevice::ReadOnly) ) ){

//        while(!_setPort.atEnd())
//        {
//            _str += _setPort.readLine();
//        }
//        _setPort.close();
//    }

//    quint16 _port;
//    QStringList _arguments = _str.split("port:", QString::SkipEmptyParts);
//    for (const auto &el : _arguments) {
//        _port = el.toUInt();
//    }

//    quint16 _port = 10002;
    Generator _generator;
//    if (!_generator.startServer(_port)){
//        qDebug() << "Error: " << _generator.errorString();
//        return 1;
//    }

//    qDebug() << "Server listen port: " << _port ;

    return a.exec();
}
