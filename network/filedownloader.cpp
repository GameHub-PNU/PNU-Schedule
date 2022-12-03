#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl fileUrl, QObject *parent) : QObject(parent)
{
    connect(&networkAccessManager, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(fileUrl);
    networkAccessManager.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    downloadedBytes = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return downloadedBytes;
}
