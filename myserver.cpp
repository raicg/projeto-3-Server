#include "myserver.h"
#include <QNetworkInterface>
MyServer::MyServer(QObject *parent) :
  QTcpServer(parent)
{

}

void MyServer::StartServer(){
  if(!this->listen(QHostAddress::Any, 1234)){
    qDebug() << "server not started";
  }
  else{
    qDebug() << "server started at:" ;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        qDebug() << address.toString();
    }
  }
}

void MyServer::incomingConnection(qintptr socketDescriptor){

  qDebug() << socketDescriptor << " connecting...";
  MyThread *thread = new MyThread(socketDescriptor,this, &storage);
  connect(thread,SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->run();
}
