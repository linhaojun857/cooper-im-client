#ifndef view_PostPyqWidget_hpp
#define view_PostPyqWidget_hpp

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PostPyqWidget;
}
QT_END_NAMESPACE

class PostPyqWidget : public QWidget {
    Q_OBJECT

public:
    explicit PostPyqWidget(QWidget* parent = nullptr);

    ~PostPyqWidget() override;

private:
    void handleClickPublishPushButton();

    void handleClickUploadPushButton();

    void changeLayoutWhenAddImage(int deltaHeight);

private:
    Ui::PostPyqWidget* ui;
    QGridLayout* m_imageLayout = nullptr;
    QList<QLabel*> m_imageLabels;
    QPushButton* m_uploadPushButton = nullptr;
    QList<QString> m_imageUrls;
};

#endif
