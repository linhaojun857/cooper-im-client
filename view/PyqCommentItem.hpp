#ifndef view_PyqCommentItem_hpp
#define view_PyqCommentItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PyqCommentItem;
}
QT_END_NAMESPACE

class PyqCommentItem : public QWidget {
    Q_OBJECT

public:
    explicit PyqCommentItem(QWidget* parent = nullptr);

    ~PyqCommentItem() override;

    void setCommonComment(const QString& nickname, const QString& comment);

    void setReplyComment(const QString& fromNickname, const QString& toNickname, const QString& comment);

private:
    Ui::PyqCommentItem* ui;
};

#endif
