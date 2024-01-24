#ifndef view_TestAudioRWWidget_hpp
#define view_TestAudioRWWidget_hpp

#include <QWidget>

#include "media/call/audio/AudioReader.hpp"
#include "media/call/audio/AudioWriter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class TestAudioRWWidget;
}
QT_END_NAMESPACE

class TestAudioRWWidget : public QWidget {
    Q_OBJECT

public:
    explicit TestAudioRWWidget(QWidget* parent = nullptr);

    ~TestAudioRWWidget() override;

private:
    Ui::TestAudioRWWidget* ui;
    AudioReader* m_audioReader = nullptr;
    AudioWriter* m_audioWriter = nullptr;
};

#endif
