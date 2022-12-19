#ifndef WEBFILEDOWNLOADER_H
#define WEBFILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class WebFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit WebFileDownloader(QObject *parent = 0);
    virtual ~WebFileDownloader();
    void sendGetHttpRequest(QUrl fileUrl);
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
