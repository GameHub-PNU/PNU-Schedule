#include "webfiledownloader.h"

WebFileDownloader::WebFileDownloader(QObject *parent) : QObject(parent)
{
    connect(&networkAccessManager, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)));
}

WebFileDownloader::~WebFileDownloader() { }

void WebFileDownloader::sendGetHttpRequest(QUrl fileUrl)
{
    QNetworkRequest request(fileUrl);
    networkAccessManager.get(request);
}

void WebFileDownloader::fileDownloaded(QNetworkReply* networkReply) {
    downloadedBytes = networkReply->readAll();
    //emit a signal
    networkReply->deleteLater();
    emit downloaded();
}

QByteArray WebFileDownloader::getDownloadedData() const {
    return downloadedBytes;
}
