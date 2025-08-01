// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_QTUTILS_H_
#define MUMBLE_MURMUR_QTUTILS_H_

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVariant>

namespace Mumble {
namespace QtUtils {

/**
 * @brief Case-insensitive QString wrapper
 * 
 * This class wraps a QString and provides case-insensitive
 * comparison and hashing functionality, which is useful for
 * maps and hashes that need to be case-insensitive.
 */
class CaseInsensitiveQString {
private:
    QString m_string;

public:
    CaseInsensitiveQString() = default;
    
    CaseInsensitiveQString(const QString &string) : m_string(string) {}
    
    bool operator==(const CaseInsensitiveQString &other) const {
        return m_string.compare(other.m_string, Qt::CaseInsensitive) == 0;
    }
    
    bool operator<(const CaseInsensitiveQString &other) const {
        return m_string.compare(other.m_string, Qt::CaseInsensitive) < 0;
    }
    
    const QString &toString() const { return m_string; }
    
    QString &toString() { return m_string; }
    
    bool isEmpty() const { return m_string.isEmpty(); }
    
    int length() const { return m_string.length(); }
    
    bool contains(const QString &str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const {
        return m_string.contains(str, cs);
    }
    
    CaseInsensitiveQString toLower() const {
        return CaseInsensitiveQString(m_string.toLower());
    }
    
    CaseInsensitiveQString toUpper() const {
        return CaseInsensitiveQString(m_string.toUpper());
    }
};

/**
 * @brief Hash function for CaseInsensitiveQString
 * 
 * @param string The string to hash
 * @return Hash value
 */
inline uint qHash(const CaseInsensitiveQString &string) {
    return qHash(string.toString().toLower());
}

/**
 * @brief Convert a QDateTime to an ISO 8601 formatted string
 * 
 * @param dateTime The QDateTime to convert
 * @return ISO 8601 formatted string
 */
inline QString dateTimeToISO8601(const QDateTime &dateTime) {
    return dateTime.toString(Qt::ISODate);
}

/**
 * @brief Convert an ISO 8601 formatted string to a QDateTime
 * 
 * @param isoString The ISO 8601 formatted string
 * @return QDateTime object
 */
inline QDateTime iso8601ToDateTime(const QString &isoString) {
    return QDateTime::fromString(isoString, Qt::ISODate);
}

/**
 * @brief Convert a QByteArray to a hex string
 * 
 * @param data The data to convert
 * @return Hex string representation
 */
inline QString byteArrayToHex(const QByteArray &data) {
    return QString(data.toHex());
}

/**
 * @brief Convert a hex string to a QByteArray
 * 
 * @param hexString The hex string to convert
 * @return QByteArray with the decoded data
 */
inline QByteArray hexToByteArray(const QString &hexString) {
    return QByteArray::fromHex(hexString.toLatin1());
}

/**
 * @brief Create a list from a comma-separated string
 * 
 * @param str The comma-separated string
 * @return QStringList containing the individual elements
 */
inline QStringList stringToList(const QString &str) {
    return str.split(',', Qt::SkipEmptyParts);
}

/**
 * @brief Join a list into a comma-separated string
 * 
 * @param list The list to join
 * @return Comma-separated string
 */
inline QString listToString(const QStringList &list) {
    return list.join(',');
}

/**
 * @brief Sanitize a string for use in file names
 * 
 * @param str The string to sanitize
 * @return Sanitized string safe for use in file names
 */
inline QString sanitizeFilename(const QString &str) {
    QString result = str;
    result.replace(QRegExp("[\\\\/:*?\"<>|]"), "_");
    return result;
}

/**
 * @brief Safely convert a variant to string with proper handling of null values
 * 
 * @param variant The variant to convert
 * @param defaultValue Default value to use if variant is null
 * @return String representation
 */
inline QString variantToString(const QVariant &variant, const QString &defaultValue = QString()) {
    return variant.isNull() ? defaultValue : variant.toString();
}

/**
 * @brief Format a byte size to a human-readable string (KB, MB, GB)
 * 
 * @param bytes The size in bytes
 * @return Human-readable size string
 */
inline QString formatByteSize(qint64 bytes) {
    const qint64 kb = 1024;
    const qint64 mb = kb * 1024;
    const qint64 gb = mb * 1024;
    
    if (bytes >= gb) {
        return QString::number(static_cast<double>(bytes) / gb, 'f', 2) + " GB";
    } else if (bytes >= mb) {
        return QString::number(static_cast<double>(bytes) / mb, 'f', 2) + " MB";
    } else if (bytes >= kb) {
        return QString::number(static_cast<double>(bytes) / kb, 'f', 2) + " KB";
    } else {
        return QString::number(bytes) + " bytes";
    }
}

} // namespace QtUtils
} // namespace Mumble

#endif // MUMBLE_MURMUR_QTUTILS_H_