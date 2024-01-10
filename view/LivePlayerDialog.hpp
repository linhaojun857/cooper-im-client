#ifndef view_PlayerDialog_hpp
#define view_PlayerDialog_hpp

#include <QDialog>
#include <QTimer>

#include "media/player/VideoPlayer.hpp"

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

private slots:
    void handleClickResumePB();

    void handleClickPausePB();

    void setImage(const QImage& image);

    void slot_PlayerStateChanged(int state);

    void slot_getTotalTime(qint64 uSec);

    void slot_TimerTimeOut();

    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::LivePlayerDialog* ui;
    VideoPlayer* m_player;
    QTimer m_timer;
    int isStop{};
};

#endif
