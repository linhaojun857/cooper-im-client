#include <QApplication>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "store/IMStore.hpp"
#include "util/HttpUtil.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QString fileName = QFileDialog::getOpenFileName(nullptr, "选择文件", "E:/test/", "All files(*.*)");
    if (fileName.isEmpty()) {
        return 0;
    }
    IMStore::getInstance()->setToken(
        "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjEiLCJpc3MiOiJjb29wZXIifQ.ffNkmzByo3zVk_t3NyG9n5iR_"
        "XUemazSPSSOKeqvyM8");
    bool ret = HttpUtil::shardUpload(fileName);
    if (ret) {
        qDebug() << "上传成功";
    } else {
        qDebug() << "上传失败";
    }
    QApplication::exec();
}
