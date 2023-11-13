#ifndef util_HTTPUTIL_HPP_
#define util_HTTPUTIL_HPP_

#include <QJsonObject>
#include <QString>

class HttpUtil {
public:
    static QJsonObject get(const QString& url);

    static QJsonObject post(const QString& url, const QJsonObject& json);

    static bool upload(const QString& filePath);

    static bool shardUpload(const QString& filePath);
};

#endif
