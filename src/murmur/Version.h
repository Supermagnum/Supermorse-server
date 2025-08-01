// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_VERSION_H_
#define MUMBLE_MURMUR_VERSION_H_

#include <QString>
#include <QRegularExpression>
#include <QtGlobal>

/**
 * @brief Version handling for Mumble
 * 
 * This namespace contains version handling functionality for
 * Mumble, including version comparison and string formatting.
 */
namespace Version {

// Version constants
static const unsigned int MAJOR = 1;
static const unsigned int MINOR = 4;
static const unsigned int PATCH = 0;

// Special version constant for unknown version
static const unsigned int UNKNOWN = 0;

// Protocol version for client-server communication
static const unsigned int PROTOCOL = 0x10205;

// Full version type
typedef quint64 full_t;

/**
 * @brief Version comparison modes
 */
enum class CompareMode {
    Equals,     ///< Equal to
    LessThan,   ///< Less than
    GreaterThan, ///< Greater than
    AtLeast,    ///< At least (equal to or greater than)
    AtMost      ///< At most (equal to or less than)
};

/**
 * @brief Create a full version number from components
 * 
 * @param major Major version number
 * @param minor Minor version number
 * @param patch Patch version number
 * @return full_t Full version number
 */
inline full_t fromComponents(unsigned int major, unsigned int minor, unsigned int patch) {
    return (static_cast<full_t>(major) << 32) | 
           (static_cast<full_t>(minor) << 16) | 
           static_cast<full_t>(patch);
}

/**
 * @brief Get the major version component
 * 
 * @param version Full version number
 * @return unsigned int Major version number
 */
inline unsigned int getMajor(full_t version) {
    return static_cast<unsigned int>((version >> 32) & 0xFFFFFFFF);
}

/**
 * @brief Get the minor version component
 * 
 * @param version Full version number
 * @return unsigned int Minor version number
 */
inline unsigned int getMinor(full_t version) {
    return static_cast<unsigned int>((version >> 16) & 0xFFFF);
}

/**
 * @brief Get the patch version component
 * 
 * @param version Full version number
 * @return unsigned int Patch version number
 */
inline unsigned int getPatch(full_t version) {
    return static_cast<unsigned int>(version & 0xFFFF);
}

/**
 * @brief Get the current version
 * 
 * @return full_t Current version
 */
inline full_t current() {
    return fromComponents(MAJOR, MINOR, PATCH);
}

/**
 * @brief Convert a version to a string
 * 
 * @param version Full version number
 * @return QString Version string (e.g., "1.4.0")
 */
inline QString toString(full_t version) {
    if (version == UNKNOWN) {
        return QStringLiteral("Unknown");
    }
    
    return QString::number(getMajor(version)) + "." + 
           QString::number(getMinor(version)) + "." + 
           QString::number(getPatch(version));
}

/**
 * @brief Compare two versions
 * 
 * @param v1 First version
 * @param v2 Second version
 * @param mode Comparison mode
 * @return bool Result of the comparison
 */
inline bool compare(full_t v1, full_t v2, CompareMode mode) {
    switch (mode) {
        case CompareMode::Equals:
            return v1 == v2;
        case CompareMode::LessThan:
            return v1 < v2;
        case CompareMode::GreaterThan:
            return v1 > v2;
        case CompareMode::AtLeast:
            return v1 >= v2;
        case CompareMode::AtMost:
            return v1 <= v2;
        default:
            return false;
    }
}

/**
 * @brief Parse a version string
 * 
 * @param versionString Version string (e.g., "1.4.0")
 * @return full_t Parsed version or UNKNOWN if parsing failed
 */
inline full_t fromString(const QString &versionString) {
    QRegularExpression re("(\\d+)\\.(\\d+)\\.(\\d+)");
    QRegularExpressionMatch match = re.match(versionString);
    
    if (match.hasMatch()) {
        unsigned int major = match.captured(1).toUInt();
        unsigned int minor = match.captured(2).toUInt();
        unsigned int patch = match.captured(3).toUInt();
        
        return fromComponents(major, minor, patch);
    }
    
    return UNKNOWN;
}

/**
 * @brief Check if a version is compatible with the current version
 * 
 * @param version Version to check
 * @return bool True if compatible
 */
inline bool isCompatible(full_t version) {
    // In a real implementation, this would check for compatibility
    // between different versions. For this simplified version,
    // we'll consider versions compatible if they have the same major version.
    return getMajor(version) == MAJOR;
}

/**
 * @brief Get a human-readable version with extra information
 * 
 * @param version Version to format
 * @return QString Formatted version string with extra information
 */
inline QString getDetailedVersionString(full_t version = current()) {
    QString result = toString(version);
    
    // Add SuperMorse version details
    result += " (SuperMorse Edition)";
    
    return result;
}

} // namespace Version

#endif // MUMBLE_MURMUR_VERSION_H_