#ifndef view_OpenLiveWidget_hpp
#define view_OpenLiveWidget_hpp

#include <QMouseEvent>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class OpenLiveWidget;
}
QT_END_NAMESPACE

class OpenLiveWidget : public QWidget {
    Q_OBJECT

public:
    explicit OpenLiveWidget(QWidget* parent = nullptr);

    ~OpenLiveWidget() override;

    bool eventFilter(QObject* watched, QEvent* event) override;

    void reset();

signals:
    void liveRoomCoverClicked();

public slots:
    void handleLiveRoomCoverClicked();

    void handleOpenLivePushButtonClicked();

private:
    Ui::OpenLiveWidget* ui;
    QString m_liveRoomCoverUrl;
};

#endif
