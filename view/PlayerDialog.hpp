#ifndef view_PlayerDialog_hpp
#define view_PlayerDialog_hpp

#include <QDialog>
#include <QTimer>

#include "media/player/VideoPlayer.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class PlayerDialog;
}
QT_END_NAMESPACE

class PlayerDialog : public QDialog {
    Q_OBJECT

public:
    explicit PlayerDialog(QWidget* parent = nullptr);

    ~PlayerDialog() override;

private slots:
    void handleClickOpenPB();

    void handleClickResumePB();

    void handleClickPausePB();

    void handleClickStopPB();

    void handleClickSetUrlPB();

    void setImage(const QImage& image);

    void slot_PlayerStateChanged(int state);

    void slot_getTotalTime(qint64 uSec);

    void slot_TimerTimeOut();

    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::PlayerDialog* ui;
    VideoPlayer* m_player;
    QTimer m_timer;
    int isStop{};
};

#endif
