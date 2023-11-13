#include <QApplication>
#include <QFileDialog>

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
    QString fileUrl = HttpUtil::upload(fileName);
    if (fileUrl.isEmpty()) {
        qDebug() << "上传失败";
    } else {
        qDebug() << "上传成功";
        qDebug() << fileUrl;
    }
    QApplication::exec();
}
