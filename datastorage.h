#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include <QDateTime>
#include <QHostAddress>
#include <QMutex>
#include <QMap>
#include <vector>

using namespace std;

typedef struct{
  QDateTime theTime; // hora da medida
  float measurement; // medida
} Entry;

class DataStorage{
public:
  DataStorage();
  vector<Entry> getData(QHostAddress address);
  void addData(QHostAddress address,
               QDateTime time,
               float measurement);

  void deleteHost(quint32 address);
  vector<QHostAddress> getHostList();

private:
  QMutex mutex;
  QMap<quint32, vector<Entry> > data;
  QMap<quint32, vector<Entry> >::iterator dataIterator;
  Q_DISABLE_COPY(DataStorage)
};

#endif // DATASTORAGE_H
