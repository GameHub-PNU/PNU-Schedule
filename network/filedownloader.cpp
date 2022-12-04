#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl fileUrl, QObject *parent) : QObject(parent)
{
    connect(&networkAccessManager, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(fileUrl);
    networkAccessManager.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* networkReply) {
    downloadedBytes = networkReply->readAll();
    //emit a signal
    networkReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::getDownloadedData() const {
    return downloadedBytes;
}
