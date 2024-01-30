#ifndef view_VideoCallRequestDialog_hpp
#define view_VideoCallRequestDialog_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoCallRequestDialog;
}
QT_END_NAMESPACE

class VideoCallRequestDialog : public QWidget {
    Q_OBJECT

public:
    explicit VideoCallRequestDialog(QWidget* parent = nullptr);

    ~VideoCallRequestDialog() override;

    void setPeerId(int peerId);

private:
    void handleClickAcceptPushButton();

    void handleClickRejectPushButton();

private:
    Ui::VideoCallRequestDialog* ui;
    int m_peerId = 0;
};

#endif
