#include <QApplication>
#include <QTime>

#include "view/PyqItem.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    PyqItem w;
    w.setContent(
        "这是测试朋友圈，这是测试朋友圈，这是测试朋友圈，这是测试朋友圈，这是测试朋友圈，这是测试朋友圈，这是测试朋友圈"
        "，这是测试朋友圈");
    //    w.setNoImage();
    QList<QString> imageUrls;
    imageUrls.append("http://localhost:9999/static/upload/1/41acb7102a864437a83c57c54e796463.jpg");
    imageUrls.append("http://localhost:9999/static/upload/1/b51728df438744e394442f5de16aee9b.jpg");
    imageUrls.append("http://localhost:9999/static/upload/1/3cf76842b29e43e0831581ec3c309917.jpg");
    w.setImages(imageUrls);
    w.setTime(QDateTime::currentDateTime().toSecsSinceEpoch());
    //    w.setNoLike(true);
    auto commentItem1 = new PyqCommentItem();
    commentItem1->setCommonComment("张三", "这是一条评论");
    auto commentItem2 = new PyqCommentItem();
    commentItem2->setReplyComment("李四", "张三", "这是一条回复");
    w.addPyqCommentItem(commentItem1);
    w.addPyqCommentItem(commentItem2);
    w.show();
    return QApplication::exec();
}
