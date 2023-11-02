#include <QApplication>
#include <QDialog>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget w;
    w.setFixedSize(800, 600);
    QLabel label(&w);
    label.setGeometry(0, 0, 200, 100);
    label.setText("hello");
    w.setToolTip("验证消息：<br>我很喜欢你的博客，我想和你探讨一下！");
    w.show();
    return QApplication::exec();
}
