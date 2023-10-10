#ifndef util_HTTPUTIL_HPP_
#define util_HTTPUTIL_HPP_

#include <QJsonObject>
#include <QString>

class HttpUtil {
public:
    static QJsonObject get(const QString& url);

    static QJsonObject post(const QString& url, const QJsonObject& json);
};

#endif
