#include <QDateTime>
#include <QString>

QString getTimeString(long long timestamp) {
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime messageTime = QDateTime::fromSecsSinceEpoch(timestamp);

    if (currentTime.date() == messageTime.date()) {
        return messageTime.toString("hh:mm");
    } else {
        QDateTime yesterday = currentTime.addDays(-1);
        if (yesterday.date() == messageTime.date()) {
            return "昨天 " + messageTime.toString("hh:mm");
        } else if (currentTime.date().year() == messageTime.date().year()) {
            return messageTime.toString("MM-dd");
        } else {
            return messageTime.toString("yy-MM-dd");
        }
    }
}

int main() {
    auto timeString = getTimeString(1698764022);
    qDebug() << timeString;
    return 0;
}
