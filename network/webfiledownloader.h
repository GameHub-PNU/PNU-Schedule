#ifndef WEBFILEDOWNLOADER_H
#define WEBFILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WebFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit WebFileDownloader(QUrl fileUrl, QObject *parent = 0);
    virtual ~WebFileDownloader();
    QByteArray getDownloadedData() const;

signals:
  void downloaded();

private slots:
  void fileDownloaded(QNetworkReply* networkReply);

private:
  QNetworkAccessManager networkAccessManager;
  QByteArray downloadedBytes;
};

#endif // WEBFILEDOWNLOADER_H
