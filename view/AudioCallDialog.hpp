#ifndef view_AudioCallDialog_hpp
#define view_AudioCallDialog_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class AudioCallDialog;
}
QT_END_NAMESPACE

class AudioCallDialog : public QWidget {
    Q_OBJECT

public:
    explicit AudioCallDialog(QWidget* parent = nullptr);

    ~AudioCallDialog() override;

private:
    Ui::AudioCallDialog* ui;
};

#endif
