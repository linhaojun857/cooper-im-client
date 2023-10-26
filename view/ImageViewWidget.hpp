#ifndef view_ImageViewWidget_hpp
#define view_ImageViewWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ImageViewWidget;
}
QT_END_NAMESPACE

class ImageViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageViewWidget(QWidget* parent = nullptr);

    ~ImageViewWidget() override;

    void clearImage();

    void setImage(const QString& url);

private:
    Ui::ImageViewWidget* ui;
};

#endif
