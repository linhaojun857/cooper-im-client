#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QWebEngineSettings>
#include <QWebEngineView>

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication a(argc, argv);
    QDialog d;
    d.setFixedSize(800, 600);
    QWebEngineView we(&d);
    we.load(QUrl("https://www.baidu.com"));
    we.setGeometry(0, 0, 800, 600);
    QWidget w;
    w.setFixedSize(200, 100);
    QPushButton pushButton1(&w);
    pushButton1.setText("打开we");
    pushButton1.setFixedSize(100, 100);
    QObject::connect(&pushButton1, &QPushButton::clicked, [&]() {
        we.show();
        d.show();
    });
    QPushButton pushButton2(&w);
    pushButton2.setText("隐藏we");
    pushButton2.setFixedSize(100, 100);
    QObject::connect(&pushButton2, &QPushButton::clicked, [&]() {
        we.hide();
        d.repaint();
        d.hide();
    });
    pushButton1.setGeometry(0, 0, 100, 100);
    pushButton2.setGeometry(100, 0, 100, 100);
    w.show();
    return QApplication::exec();
}
