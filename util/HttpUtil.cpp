#include "HttpUtil.hpp"

#include <QEventLoop>
#include <QFile>
#include <QHttpPart>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"

QJsonObject HttpUtil::get(const QString& url) {
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Connection", "close");
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
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      QVariant(QJsonDocument(json).toJson(QJsonDocument::Compact).size()));
    request.setRawHeader("Connection", "close");
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

QString HttpUtil::upload(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }
    auto fileMd5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
    QString fileName = filePath.mid(filePath.lastIndexOf("/") + 1);
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["filename"] = fileName;
    json["file_md5"] = QString(fileMd5);
    auto cRet = HttpUtil::post(HTTP_SERVER_URL "/file/checkBeforeUpload", json);
    if (cRet["code"].toInt() == HTTP_SUCCESS_CODE) {
        if (cRet["exist"].toInt() == 1) {
            qDebug() << "file exist";
            return cRet["file_url"].toString();
        }
    } else {
        return "";
    }
    file.seek(0);
    QByteArray data = file.readAll();
    file.close();
    auto* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(R"(form-data; name="file"; filename=")" + fileName + "\""));
    filePart.setBody(data);
    QHttpPart fileMd5Part;
    fileMd5Part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="file_md5")"));
    fileMd5Part.setBody(fileMd5);
    multiPart->append(filePart);
    multiPart->append(fileMd5Part);
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(HTTP_SERVER_URL "/file/upload"));
    request.setRawHeader("Connection", "close");
    request.setRawHeader("token", IMStore::getInstance()->getToken().toUtf8());
    auto reply = manager->post(request, multiPart);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        return "";
    }
    QJsonObject ret = QJsonDocument::fromJson(reply->readAll()).object();
    qDebug() << ret;
    if (ret["code"].toInt() == HTTP_SUCCESS_CODE) {
        return ret["file_url"].toString();
    } else {
        return "";
    }
}

QString HttpUtil::shardUpload(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }
    QString fileName = filePath.mid(filePath.lastIndexOf("/") + 1);
    auto fileMd5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["filename"] = fileName;
    json["file_md5"] = QString(fileMd5);
    auto cRet = HttpUtil::post(HTTP_SERVER_URL "/file/checkBeforeUpload", json);
    if (cRet["code"].toInt() == HTTP_SUCCESS_CODE) {
        if (cRet["exist"].toInt() == 1) {
            qDebug() << "file exist";
            return cRet["file_url"].toString();
        }
    } else {
        return "";
    }
    file.seek(0);
    int shardSize = 1024 * 1024;
    int shardCount = static_cast<int>(file.size()) / shardSize;
    if (file.size() % shardSize != 0) {
        ++shardCount;
    }
    std::vector<QByteArray> shards(shardCount);
    for (int i = 0; i < shardCount; ++i) {
        file.seek(i * shardSize);
        if (i == shardCount - 1) {
            shards[i] = file.readAll();
        } else {
            shards[i] = file.read(shardSize);
        }
    }
    for (int i = 0; i < shardCount; ++i) {
        qDebug() << "shard" << i << "size:" << shards[i].size();
    }
    QVector<QFuture<void>> uploadFutures(shardCount);
    QString suId = QUuid::createUuid().toString().toUtf8();
    suId.replace(0, 1, "").replace(suId.size() - 1, 1, "").replace("-", "");
    QString fileUrl;
    for (int i = 0; i < shardCount; ++i) {
        uploadFutures[i] = QtConcurrent::run([=, &shards, &fileUrl]() {
            auto* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
            QHttpPart filePart;
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant(R"(form-data; name="file"; filename=")" + fileName + "\""));
            filePart.setBody(shards[i]);
            QHttpPart suIdPart;
            suIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="su_id")"));
            suIdPart.setBody(suId.toUtf8());
            QHttpPart shardIndexPart;
            shardIndexPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                     QVariant(R"(form-data; name="shard_index")"));
            shardIndexPart.setBody(QString::number(i).toUtf8());
            QHttpPart shardSizePart;
            shardSizePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                    QVariant(R"(form-data; name="shard_size")"));
            shardSizePart.setBody(QString::number(shardSize).toUtf8());
            QHttpPart shardCountPart;
            shardCountPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                     QVariant(R"(form-data; name="shard_count")"));
            shardCountPart.setBody(QString::number(shardCount).toUtf8());
            QHttpPart fileMd5Part;
            fileMd5Part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="file_md5")"));
            fileMd5Part.setBody(fileMd5);
            multiPart->append(filePart);
            multiPart->append(suIdPart);
            multiPart->append(shardIndexPart);
            multiPart->append(shardSizePart);
            multiPart->append(shardCountPart);
            multiPart->append(fileMd5Part);
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request;
            request.setUrl(QUrl(HTTP_SERVER_URL "/file/shardUpload"));
            request.setRawHeader("Connection", "close");
            request.setRawHeader("token", IMStore::getInstance()->getToken().toUtf8());
            auto reply = manager->post(request, multiPart);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << reply->errorString();
                return;
            }
            QJsonObject ret = QJsonDocument::fromJson(reply->readAll()).object();
            qDebug() << ret;
            if (ret["code"].toInt() == HTTP_SUCCESS_CODE && ret["complete"].toInt() == 1) {
                fileUrl = ret["file_url"].toString();
            }
        });
    }
    for (auto& future : uploadFutures) {
        future.waitForFinished();
    }
    return fileUrl;
}
