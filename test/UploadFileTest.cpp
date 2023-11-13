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
    QString fileName = QFileDialog::getOpenFileName(nullptr, "选择文件", ".", "All files(*.*)");
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
    QByteArray data = file.readAll();
    file.close();
    // 使用QHttpMultiPart上传文件，image/png
    auto* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(R"(form-data; name="file"; filename=")" + fileName + "\""));
    filePart.setBody(data);
    multiPart->append(filePart);
    // 使用QNetworkAccessManager发送请求
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(HTTP_SERVER_URL "/file/upload"));
    request.setRawHeader("Connection", "close");
    // 设置token到header
    request.setRawHeader("token",
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
    QApplication::exec();
}
