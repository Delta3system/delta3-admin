#pragma once

#include "abstrproto.h"

#if defined(DELTA3_ADMIN_LIBRARY)
#   define DELTA3_EXPORT Q_DECL_EXPORT
#else
#   define DELTA3_EXPORT Q_DECL_IMPORT
#endif


class QTcpServer;
class QTcpSocket;

namespace delta3 {

class DELTA3_EXPORT Proxy : public AbstrProto
{
    Q_OBJECT
public:
    explicit Proxy(Network *net = NULL,
                   qint16 clientId = 0,
                   QObject *parent = 0);
    void start(int port);
    void stop();

protected slots:
    void onDataReceived();

private:
    QTcpServer *proxyServer_;
    QTcpSocket *socket_;

private slots:
    void slotReadyRead();
    void slotNewConnection();
};

}
