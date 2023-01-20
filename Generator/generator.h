#ifndef GENERATOR_H
#define GENERATOR_H

#include <QTcpServer>
#include <QUdpSocket>
#include <QTimer>

class Generator : public QTcpServer
{
    Q_OBJECT
    QTimer *m_timerSend;
    QUdpSocket *m_socket;

    int m_amplitude{20000000};
public:
    Generator();
    ~Generator();

//    bool startServer(quint16 _port);

protected:
//    void incomingConnection(qintptr _handle);

private slots:
};

#endif // GENERATOR_H
