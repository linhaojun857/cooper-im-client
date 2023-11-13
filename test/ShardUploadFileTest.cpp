#include <QApplication>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // 弹出文件选择框，选择上传文件
    QString fileName = QFileDialog::getOpenFileName(nullptr, "选择文件", "E:/test/", "All files(*.*)");
    if (fileName.isEmpty()) {
        return 0;
    }
    // 读取文件内容
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "提示", "文件打开失败");
        return 0;
    }
    fileName = fileName.mid(fileName.lastIndexOf("/") + 1);
    auto fileMd5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
    qDebug() << "fileMd5:" << fileMd5;
    file.seek(0);
    // 将文件进行分片 1MB数据为一片
    int shardSize = 1024 * 1024;
    int shardCount = static_cast<int>(file.size()) / shardSize;
    if (file.size() % shardSize != 0) {
        ++shardCount;
    }
    // 开对应数量的线程进行分片
    std::vector<QByteArray> shards(shardCount);
    for (int i = 0; i < shardCount; ++i) {
        file.seek(i * shardSize);
        if (i == shardCount - 1) {
            shards[i] = file.readAll();
        } else {
            shards[i] = file.read(shardSize);
        }
    }
    // 打印各个分片的信息
    for (int i = 0; i < shardCount; ++i) {
        qDebug() << "shard" << i << "size:" << shards[i].size();
    }
    file.close();
    // 开对应数量的线程进行上传
    QVector<QFuture<void>> uploadFutures(shardCount);
    QString suId = QUuid::createUuid().toString().toUtf8();
    suId.replace(0, 1, "").replace(suId.size() - 1, 1, "").replace("-", "");
    for (int i = 0; i < shardCount; ++i) {
        uploadFutures[i] = QtConcurrent::run([=, &shards]() {
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
            // 使用QNetworkAccessManager发送请求
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request;
            request.setUrl(QUrl(HTTP_SERVER_URL "/file/shardUpload"));
            request.setRawHeader("Connection", "close");
            // 设置token到header
            request.setRawHeader(
                "token",
                "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjEiLCJpc3MiOiJjb29wZXIifQ.ffNkmzByo3zVk_"
                "t3NyG9n5iR_XUemazSPSSOKeqvyM8");
            auto reply = manager->post(request, multiPart);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << reply->errorString();
                return 0;
            }
            qDebug() << QJsonDocument::fromJson(reply->readAll()).object();
        });
    }
    for (auto& future : uploadFutures) {
        future.waitForFinished();
    }
    QApplication::exec();
}
