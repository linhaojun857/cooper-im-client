#ifndef view_PlayerDialog_hpp
#define view_PlayerDialog_hpp

#include <QDialog>
#include <QTimer>
#include <QVBoxLayout>

#include "media/player/VideoPlayer.hpp"
#include "view/LiveRoomMsgItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class LivePlayerDialog;
}
QT_END_NAMESPACE

class LivePlayerDialog : public QDialog {
    Q_OBJECT

public:
    explicit LivePlayerDialog(QWidget* parent = nullptr);

    ~LivePlayerDialog() override;

    void start(int roomId);

    void addLiveRoomMsgItem(LiveRoomMsgItem* liveRoomMsgItem);

private slots:
    void handleClickResumePB();

    void handleClickPausePB();

    void setImage(const QImage& image);

    void slot_PlayerStateChanged(int state);

    void slot_getTotalTime(qint64 uSec);

    void slot_TimerTimeOut();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void handleClickSendMsgPB();

private:
    Ui::LivePlayerDialog* ui;
    VideoPlayer* m_player;
    QTimer m_timer;
    QVBoxLayout* m_msgLayout = nullptr;
};

#endif
