#include "generator.h"
#include <QRandomGenerator>

Generator::Generator()
{
    m_socket  = new QUdpSocket(this);

    m_timerSend = new QTimer;
    connect(m_timerSend, &QTimer::timeout, [&]() {

        QString word(QString::number(QRandomGenerator::global() -> bounded(0, m_amplitude)).toLatin1());
        QByteArray buffer;
        buffer=word.toUtf8();

        m_socket -> writeDatagram(buffer.data(), QHostAddress::LocalHost, 10002 );
    });

    m_timerSend -> start(200);

}

Generator::~Generator()
{

}

/*
bool Generator::startServer(quint16 _port)
{
    return listen(QHostAddress::Any, _port);
}

void Generator::incomingConnection(qintptr _handle)
{
    m_socket = new QUdpSocket(this);
    m_socket -> bind(QHostAddress::LocalHost, 10002);



    connect(m_socket, &QUdpSocket::readyRead, [&]() {
        QTextStream T(m_socket);
        auto text = T.readAll();

        if (text == "0x01")
            m_timerSend -> start(200);
        else if (text == "0x00") {
            if (m_timerSend -> isActive())
                m_timerSend -> stop();
        }
        //        m_amplitude = text.toInt();
    });

    connect(m_socket, &QUdpSocket::disconnected, [&]() {
        if (m_timerSend -> isActive())
            m_timerSend -> stop();
    });

    if (m_socket->setSocketDescriptor(_handle)) {
        qDebug() << "Client connected with handle: " << _handle;
    }
}
*/
