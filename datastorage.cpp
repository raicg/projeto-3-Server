#include "datastorage.h"
#include <QMutexLocker>
#include <QDebug>

// constructor
DataStorage::DataStorage() : mutex()
{
}

vector<Entry> DataStorage::getData(QHostAddress address)
{
  // locks the mutex so
  QMutexLocker ml(&mutex);
  dataIterator = data.find(address.toIPv4Address());
  if(dataIterator != data.end()){
    return(dataIterator.value());
  }
  else{
    return(vector<Entry>());
  }
}

void DataStorage::addData(QHostAddress address, QDateTime time, float measurement){
  Entry entry;

  QMutexLocker ml(&mutex);

  entry.theTime = time;
  entry.measurement = measurement;
  dataIterator = data.find(address.toIPv4Address());
  if(dataIterator != data.end()){
    qDebug() << "passou: " << address.toIPv4Address() ;
    data[address.toIPv4Address()].push_back(entry);
    qDebug() << "data.size" << data[address.toIPv4Address()].size();
  }
  else{
    vector<Entry> start;
    start.push_back(entry);
    data.insert(address.toIPv4Address(), start);
  }
  qDebug() << "saiu addData";
}

vector<QHostAddress> DataStorage::getHostList()
{
  vector<QHostAddress> hostList;
  QList<quint32> uintList;
  uintList = data.keys();
  for(int i=0; i<uintList.size(); i++){
    hostList.push_back(QHostAddress(uintList[i]));
  }
  return hostList;
}













