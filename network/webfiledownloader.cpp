#include "webfiledownloader.h"

WebFileDownloader::WebFileDownloader(QUrl fileUrl, QObject *parent) : QObject(parent)
{
    connect(&networkAccessManager, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(fileUrl);
    networkAccessManager.get(request);
}

WebFileDownloader::~WebFileDownloader() { }

void WebFileDownloader::fileDownloaded(QNetworkReply* networkReply) {
    downloadedBytes = networkReply->readAll();
    //emit a signal
    networkReply->deleteLater();
    emit downloaded();
}

QByteArray WebFileDownloader::getDownloadedData() const {
    return downloadedBytes;
}
