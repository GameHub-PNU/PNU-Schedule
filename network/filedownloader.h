#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl fileUrl, QObject *parent = 0);
    virtual ~FileDownloader();
    QByteArray getDownloadedData() const;

signals:
  void downloaded();

private slots:
  void fileDownloaded(QNetworkReply* networkReply);

private:
  QNetworkAccessManager networkAccessManager;
  QByteArray downloadedBytes;
};

#endif // FILEDOWNLOADER_H
