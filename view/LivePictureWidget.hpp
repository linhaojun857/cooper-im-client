#ifndef view_PictureWidget_hpp
#define view_PictureWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LivePictureWidget;
}
QT_END_NAMESPACE

class LivePictureWidget : public QWidget {
    Q_OBJECT

public:
    explicit LivePictureWidget(QWidget* parent = nullptr);

    ~LivePictureWidget() override;

public slots:
    void slots_setImage(const QImage& image);

private:
    Ui::LivePictureWidget* ui;
};

#endif
