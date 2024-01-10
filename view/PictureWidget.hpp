#ifndef view_PictureWidget_hpp
#define view_PictureWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PictureWidget;
}
QT_END_NAMESPACE

class PictureWidget : public QWidget {
    Q_OBJECT

public:
    explicit PictureWidget(QWidget* parent = nullptr);

    ~PictureWidget() override;

public slots:
    void slots_setImage(const QImage& image);

private:
    Ui::PictureWidget* ui;
};

#endif
