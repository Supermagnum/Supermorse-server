// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserStatisticsModule.h"
#include "../Server.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>

UserStatisticsModule::UserStatisticsModule(QObject *parent)
    : IServerModule(parent)
    , m_server(nullptr) {
    // Initialize the statistics directory path
    m_statsDirectory = QDir::current();
    m_statsDirectory.cdUp(); // Move up one level from the binary directory
    m_statsDirectory.cd("user-stats"); // Try to move into the user-stats directory
}

UserStatisticsModule::~UserStatisticsModule() {
    // Cleanup will be done in shutdown()
}

bool UserStatisticsModule::initialize(Server *server) {
    m_server = server;
    
    // Create the user-stats directory if it doesn't exist
    bool directoryExists = ensureDirectoryExists();
    
    if (directoryExists) {
        qDebug() << "UserStatisticsModule: Initialized with stats directory at" << m_statsDirectory.absolutePath();
    } else {
        qWarning() << "UserStatisticsModule: Failed to create stats directory at" << m_statsDirectory.absolutePath();
    }
    
    // Emit the directory status signal
    emit directoryStatusChanged(directoryExists);
    
    return directoryExists;
}

QString UserStatisticsModule::name() const {
    return "UserStatistics";
}

QString UserStatisticsModule::version() const {
    return "1.0.0";
}

QString UserStatisticsModule::description() const {
    return "Manages the collection and storage of user statistics data";
}

QVariant UserStatisticsModule::getSetting(const QString &key, const QVariant &defaultValue) const {
    QMutexLocker locker(const_cast<QRecursiveMutex *>(&m_mutex));
    
    // Handle specific settings
    if (key == "statsDirectoryPath") {
        return m_statsDirectory.absolutePath();
    }
    
    // Return default value for unknown keys
    return defaultValue;
}

bool UserStatisticsModule::setSetting(const QString &key, const QVariant &value) {
    QMutexLocker locker(&m_mutex);
    
    // Handle specific settings
    if (key == "statsDirectoryPath") {
        QString newPath = value.toString();
        QDir newDir(newPath);
        
        if (newDir.exists() || newDir.mkpath(".")) {
            m_statsDirectory = newDir;
            qDebug() << "UserStatisticsModule: Stats directory path changed to" << newPath;
            return true;
        } else {
            qWarning() << "UserStatisticsModule: Failed to set stats directory path to" << newPath;
            return false;
        }
    }
    
    // Unknown setting
    return false;
}

void UserStatisticsModule::shutdown() {
    qDebug() << "UserStatisticsModule: Shutting down";
    
    // Clean up resources if needed
    m_server = nullptr;
}

QString UserStatisticsModule::getStatsDirectoryPath() const {
    QMutexLocker locker(const_cast<QRecursiveMutex *>(&m_mutex));
    return m_statsDirectory.absolutePath();
}

bool UserStatisticsModule::processUserStatsFile(const QString &userName, const QString &fileName, const QByteArray &fileData) {
    QMutexLocker locker(&m_mutex);
    
    // First, ensure the directory exists
    if (!ensureDirectoryExists()) {
        qWarning() << "UserStatisticsModule: Failed to create stats directory, cannot save file" << fileName;
        return false;
    }
    
    // Create user-specific directory if needed
    if (!ensureUserDirectoryExists(userName)) {
        qWarning() << "UserStatisticsModule: Failed to create user directory for" << userName;
        return false;
    }
    
    // Validate the file content
    if (!validateStatsFile(fileData)) {
        qWarning() << "UserStatisticsModule: Invalid statistics file content for" << fileName;
        return false;
    }
    
    // Construct the file path
    QString userDirPath = m_statsDirectory.absolutePath() + "/" + userName;
    QString filePath = userDirPath + "/" + fileName;
    
    // Process the content to ensure it follows the required format
    QString content = QString::fromUtf8(fileData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);
    
    // Prepare formatted lines
    QStringList formattedLines;
    
    // Always keep the header
    QString header = lines.first();
    formattedLines.append(header);
    
    // Process each data line to ensure proper format:
    // username, characters learned, time per character, features unlocked, emailadress
    for (int i = 1; i < lines.size(); i++) {
        QStringList fields = lines[i].split(',');
        
        // Ensure we have at least 5 fields
        while (fields.size() < 5) {
            fields.append("");
        }
        
        // Ensure character and time fields are properly formatted
        // Example: Characters "K M" should have times "3 15"
        QString charactersLearned = fields[1].trimmed();
        QString timePerCharacter = fields[2].trimmed();
        
        if (!charactersLearned.isEmpty()) {
            QStringList characters = charactersLearned.split(' ', Qt::SkipEmptyParts);
            QStringList times = timePerCharacter.split(' ', Qt::SkipEmptyParts);
            
            // Adjust time entries to match character count if needed
            while (times.size() < characters.size()) {
                times.append("0"); // Default time value
            }
            
            // Trim excess time entries
            while (times.size() > characters.size()) {
                times.removeLast();
            }
            
            // Update the fields with properly formatted data
            fields[1] = characters.join(" ");
            fields[2] = times.join(" ");
        }
        
        // Add the formatted line
        formattedLines.append(fields.join(","));
    }
    
    // Join all lines with newlines
    QString formattedContent = formattedLines.join("\n");
    QByteArray formattedData = formattedContent.toUtf8();
    
    // Write the formatted file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "UserStatisticsModule: Failed to open file for writing:" << filePath;
        return false;
    }
    
    qint64 bytesWritten = file.write(formattedData);
    file.close();
    
    if (bytesWritten != formattedData.size()) {
        qWarning() << "UserStatisticsModule: Failed to write complete file content to" << filePath;
        return false;
    }
    
    qDebug() << "UserStatisticsModule: Successfully wrote statistics file" << fileName << "for user" << userName;
    
    // Emit the signal
    emit userStatsReceived(userName, fileName);
    
    return true;
}

bool UserStatisticsModule::hasUserStats(const QString &userName) const {
    QMutexLocker locker(const_cast<QRecursiveMutex *>(&m_mutex));
    
    QString userDirPath = m_statsDirectory.absolutePath() + "/" + userName;
    QDir userDir(userDirPath);
    
    return userDir.exists() && !userDir.entryList(QDir::Files).isEmpty();
}

QStringList UserStatisticsModule::getUserStatsFiles(const QString &userName) const {
    QMutexLocker locker(const_cast<QRecursiveMutex *>(&m_mutex));
    
    QString userDirPath = m_statsDirectory.absolutePath() + "/" + userName;
    QDir userDir(userDirPath);
    
    if (!userDir.exists()) {
        return QStringList();
    }
    
    return userDir.entryList(QDir::Files, QDir::Time);
}

bool UserStatisticsModule::ensureDirectoryExists() {
    // Check if the directory exists
    if (m_statsDirectory.exists()) {
        return true;
    }
    
    // Directory doesn't exist, create it
    QString path = QDir::current().absolutePath();
    path = path.section('/', 0, -2) + "/user-stats"; // Go up one level and add 'user-stats'
    
    bool created = QDir().mkpath(path);
    if (created) {
        m_statsDirectory = QDir(path);
        qDebug() << "UserStatisticsModule: Created stats directory at" << path;
    } else {
        qWarning() << "UserStatisticsModule: Failed to create stats directory at" << path;
    }
    
    return created;
}

bool UserStatisticsModule::ensureUserDirectoryExists(const QString &userName) {
    QString userDirPath = m_statsDirectory.absolutePath() + "/" + userName;
    QDir userDir(userDirPath);
    
    if (userDir.exists()) {
        return true;
    }
    
    bool created = QDir().mkpath(userDirPath);
    if (created) {
        qDebug() << "UserStatisticsModule: Created user directory for" << userName;
    } else {
        qWarning() << "UserStatisticsModule: Failed to create user directory for" << userName;
    }
    
    return created;
}

bool UserStatisticsModule::validateStatsFile(const QByteArray &fileData) {
    // Basic validation - check if it's a CSV file with expected headers
    QString content = QString::fromUtf8(fileData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);
    
    // Check if we have at least one line
    if (lines.isEmpty()) {
        qWarning() << "UserStatisticsModule: Empty file content";
        return false;
    }
    
    // Check if the first line has required headers for the format:
    // username, characters learned, time per character, features unlocked, emailadress
    QString header = lines.first().toLower();
    
    if (!header.contains("username") || 
        !header.contains("characters learned") || 
        !header.contains("time per character") || 
        !header.contains("features unlocked") || 
        !header.contains("emailadress")) {
        
        qWarning() << "UserStatisticsModule: Missing required headers in CSV file";
        return false;
    }
    
    // For data lines, verify format matches expected structure
    if (lines.size() > 1) {
        for (int i = 1; i < lines.size(); i++) {
            QStringList fields = lines[i].split(',');
            
            // Check we have enough fields
            if (fields.size() < 5) {
                qWarning() << "UserStatisticsModule: Line" << i+1 << "has fewer fields than required";
                return false;
            }
            
            // Verify characters learned and time per character consistency
            QString charactersLearned = fields[1].trimmed();
            QString timePerCharacter = fields[2].trimmed();
            
            if (!charactersLearned.isEmpty()) {
                QStringList characters = charactersLearned.split(' ', Qt::SkipEmptyParts);
                QStringList times = timePerCharacter.split(' ', Qt::SkipEmptyParts);
                
                if (characters.size() != times.size()) {
                    qWarning() << "UserStatisticsModule: Line" << i+1 
                               << "has mismatched characters and time values count";
                    return false;
                }
            }
        }
    }
    
    return true;
}