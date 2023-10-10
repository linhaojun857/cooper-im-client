#include "HttpUtil.hpp"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QJsonObject HttpUtil::get(const QString& url) {
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    auto reply = manager->get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        return {};
    }
    auto byteArray = reply->readAll();
    return QJsonDocument::fromJson(byteArray).object();
}

QJsonObject HttpUtil::post(const QString& url, const QJsonObject& json) {
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    auto reply = manager->post(request, QJsonDocument(json).toJson(QJsonDocument::Compact));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        return {};
    }
    auto byteArray = reply->readAll();
    return QJsonDocument::fromJson(byteArray).object();
}
