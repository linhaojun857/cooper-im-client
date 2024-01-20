#ifndef view_LiveRoomItem_hpp
#define view_LiveRoomItem_hpp

#include <QMouseEvent>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LiveRoomItem;
}
QT_END_NAMESPACE

class LiveRoomItem : public QWidget {
    Q_OBJECT

public:
    explicit LiveRoomItem(QWidget* parent = nullptr);

    ~LiveRoomItem() override;

    bool eventFilter(QObject* watched, QEvent* event) override;

    void setLiveRoomId(int liveRoomId);

    void setLiveRoomOwnerName(const QString& liveRoomOwnerName);

    void setLiveRoomCover(const QString& liveRoomCoverUrl);

    void setLiveRoomOwnerAvatar(const QString& liveRoomOwnerAvatarUrl);

    void setLiveRoomViewerCount(int liveRoomViewerCount);

signals:
    void liveRoomCoverClicked();

public slots:
    void handleLiveRoomCoverClicked() const;

private:
    Ui::LiveRoomItem* ui;
    int m_liveRoomId = 0;
    QString m_liveRoomOwnerName;
    QString m_liveRoomCoverUrl;
    QString m_liveRoomOwnerAvatarUrl;
    int m_liveRoomViewerCount = 0;
};

#endif
