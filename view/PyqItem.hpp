#ifndef view_PyqItem_hpp
#define view_PyqItem_hpp

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PyqItem;
}
QT_END_NAMESPACE

class PyqItem : public QWidget {
    Q_OBJECT

public:
    explicit PyqItem(QWidget* parent = nullptr);

    ~PyqItem() override;

    void setAvatar(const QString& avatarUrl);

    void setNickname(const QString& nickname);

    void setContent(const QString& pyqContent);

    void setImages(const QList<QString>& imageUrls);

    void setTime(long long timestamp);

public slots:
    void handleClickDeletePushButton();

    void handleClickLikePushButton();

    void handleClickCommentPushButton();

private:
    void setImages3(const QList<QString>& imageUrls);

    void setImages4(const QList<QString>& imageUrls);

    void setImages6(const QList<QString>& imageUrls);

    void setImages9(const QList<QString>& imageUrls);

    void changeLayoutWhenSetImages(int deltaHeight);

private:
    Ui::PyqItem* ui;
    QList<QLabel*> m_imageLabels;
    QGridLayout* m_imageLabelsLayout = nullptr;
};

#endif
