#include <QApplication>
#include <QTime>

#include "view/PyqWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    PyqWidget w;
    for (int i = 0; i < 20; i++) {
        auto* item = new PyqItem();
        QString content;
        for (int j = 0; j < 9; j++) {
            if (j == 8) {
                content += QString("这是测试朋友圈%1").arg(i);
            } else {
                content += QString("这是测试朋友圈%1，").arg(i);
            }
        }
        item->setContent(content);
        QList<QString> imageUrls;
        imageUrls.append("http://localhost:9999/static/upload/1/41acb7102a864437a83c57c54e796463.jpg");
        imageUrls.append("http://localhost:9999/static/upload/1/b51728df438744e394442f5de16aee9b.jpg");
        imageUrls.append("http://localhost:9999/static/upload/1/3cf76842b29e43e0831581ec3c309917.jpg");
        item->setImages(imageUrls);
        item->setTime(QDateTime::currentDateTime().toSecsSinceEpoch());
        auto commentItem1 = new PyqCommentItem();
        commentItem1->setCommonComment("张三",
                                       "这是一条评论，这是一条评论，这是一条评论，这是一条评论，这是一条评论，这是一条"
                                       "评论，这是一条评论，这是一条评论，这是一条评论");
        auto commentItem2 = new PyqCommentItem();
        commentItem2->setReplyComment("李四", "张三", "这是一条回复");
        item->addPyqCommentItem(commentItem1);
        item->addPyqCommentItem(commentItem2);
        w.addPyqItem(item);
    }
    w.show();
    return QApplication::exec();
}
