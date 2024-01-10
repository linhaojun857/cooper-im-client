#ifndef view_RecordDialog_hpp
#define view_RecordDialog_hpp

#include <QDialog>

#include "media/recorder/SaveVideoFileThread.hpp"
#include "view/PictureWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class RecordDialog;
}
QT_END_NAMESPACE

class RecordDialog : public QDialog {
    Q_OBJECT

public:
    explicit RecordDialog(QWidget* parent = nullptr);

    ~RecordDialog() override;

public slots:
    void slots_setImage(const QImage& image);

private:
    Ui::RecordDialog* ui;
    PictureWidget* m_pictureWidget = nullptr;
    SaveVideoFileThread* m_saveVideoFileThread = nullptr;
    QString m_saveUrl;
};

#endif
