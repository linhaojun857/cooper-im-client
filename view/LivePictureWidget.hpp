#ifndef view_PictureWidget_hpp
#define view_PictureWidget_hpp

#include <QMouseEvent>
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

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::LivePictureWidget* ui;
    QPointF m_dragStartPosition;
    bool m_isDragging{};
    bool m_adjustFlag = true;
};

#endif
