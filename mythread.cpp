#include "mythread.h"
#include <vector>

using namespace std;

MyThread::MyThread(int ID, QObject *parent, DataStorage* storage) :
  QThread (parent)
{
  this->socketDescriptor = ID;
  this->storage = storage;
}

void MyThread::run()
{
  // thread starts here
  qDebug() << socketDescriptor << " starting thread";
  socket = new QTcpSocket();
  if(!socket->setSocketDescriptor(this->socketDescriptor)){
    emit error(socket->error());
    return;
  }
  // direct connection slot invocado diretamente
  connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
  connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

  qDebug() << socketDescriptor << " client connected";
  peerAddress = socket->peerAddress();
  qDebug() << peerAddress.toString();
  exec();
}

void MyThread::readyRead(){
  vector<Entry> entry;
  vector<QHostAddress> hostList;
  Entry value;
  QDateTime datetime;

  QString str = socket->readLine().replace("\n","").replace("\r","");
  QStringList list;

  list = str.split(" ");
  // parses the string list
  //storage->addData(peerAddress,QDateTime::currentDateTime(), 23);

  QString cmd;
  cmd = list.at(0);

  // lista os hosts produtores de dados
  if(cmd == "list"){
    hostList = storage->getHostList();
    if(hostList.size() > 0){
      for(int i=0; i<hostList.size(); i++){
        socket->write(hostList[i].toString().toStdString().c_str());
        socket->write("\r\n");
      }
    }
    else{
      socket->write("no hosts available");
    }
    if(hostList.size() == 0){
      socket->write("\r\n");
    }
    else{
      for(int i=0; i<hostList.size(); i++){
        socket->write(hostList[i].toString().toStdString().c_str());
        socket->write("\r\n");
      }
    }
  }

  // lista as entradas de um dado host
  else if(cmd == "get"){
    // recupera o argumento fornecido para get
    // ex: get 127.0.0.1
    if(list.size() == 2){
      cmd = list.at(1);
//      qDebug() << "pass 1";
      QHostAddress address(cmd);
      // se o endereco for valido...
      if(!address.isNull()){
        // recupera entradas para um dado endereco
        entry = storage->getData(address);
        // mostra as entradas para o cliente
        for(int i=0; i<entry.size(); i++){
          value = entry[i];
          socket->write(value.theTime.toString(Qt::ISODate).toStdString().c_str());
          socket->write(" ");
          socket->write(QString().number(value.measurement).toStdString().c_str());
          socket->write("\n");
        }
      }
    }
  }

  // insere dados de telemetria
  else if(cmd == "set"){
    // sintaxe: set date time float_value
    // ex: set 2016-05-04T10:24:14 34
    if(list.size() == 3){
      cmd = list.at(1);
      datetime = QDateTime::fromString(cmd, Qt::ISODate);
      if(datetime.isValid()){
        cmd = list.at(2);
        bool ok;
        value.measurement = cmd.toFloat(&ok);
        if(ok){
          qDebug() << "escreveu: " << datetime << value.measurement;
          storage->addData(socket->peerAddress(),datetime,
                           value.measurement);
        }
      }
    }
  }

}

void MyThread::disconnected(){
  qDebug() << socketDescriptor << "Disconnected";
  socket->deleteLater();
  // diz a thread para parar
  exit(0);
}
