#ifndef view_RecordDialog_hpp
#define view_RecordDialog_hpp

#include <QDialog>

#include "media/recorder/SaveVideoFileThread.hpp"
#include "view/LivePictureWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class LiveRecordDialog;
}
QT_END_NAMESPACE

class LiveRecordDialog : public QDialog {
    Q_OBJECT

public:
    explicit LiveRecordDialog(QWidget* parent = nullptr);

    ~LiveRecordDialog() override;

    void start(int roomId);

public slots:
    void slots_setImage(const QImage& image);

    void handleClickPbPause();

    void handleClickPbEnd();

private:
    Ui::LiveRecordDialog* ui;
    LivePictureWidget* m_pictureWidget = nullptr;
    SaveVideoFileThread* m_saveVideoFileThread = nullptr;
    QString m_saveUrl;
    bool m_adjustFlag = true;
};

#endif
