// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_USERSTATISTICSMODULE_H_
#define MUMBLE_MURMUR_USERSTATISTICSMODULE_H_

#include "IServerModule.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTimer>

class Server;
class ServerUser;

/**
 * @brief The UserStatisticsModule class manages user statistics collection and storage.
 * 
 * This module handles the reception and storage of user statistics from clients,
 * ensuring the 'user-stats' directory exists and managing incoming CSV files.
 */
class UserStatisticsModule : public IServerModule {
    Q_OBJECT
public:
    /**
     * @brief Constructor for UserStatisticsModule.
     * @param parent The parent QObject
     */
    explicit UserStatisticsModule(QObject *parent = nullptr);
    
    /**
     * @brief Destructor for UserStatisticsModule.
     */
    ~UserStatisticsModule();
    
    // IServerModule interface implementation
    bool initialize(Server *server) override;
    QString name() const override;
    QString version() const override;
    QString description() const override;
    QVariant getSetting(const QString &key, const QVariant &defaultValue = QVariant()) const override;
    bool setSetting(const QString &key, const QVariant &value) override;
    void shutdown() override;
    
    /**
     * @brief Get the path to the user statistics directory.
     * 
     * @return The path to the user statistics directory
     */
    QString getStatsDirectoryPath() const;
    
    /**
     * @brief Process an incoming user statistics file from a client.
     * 
     * @param userName The username associated with the statistics
     * @param fileName The name of the statistics file
     * @param fileData The content of the statistics file
     * @return True if the file was processed successfully, false otherwise
     */
    bool processUserStatsFile(const QString &userName, const QString &fileName, const QByteArray &fileData);
    
    /**
     * @brief Check if a user has any statistics files.
     * 
     * @param userName The username to check
     * @return True if the user has statistics files, false otherwise
     */
    bool hasUserStats(const QString &userName) const;
    
    /**
     * @brief Get the list of statistics files for a user.
     * 
     * @param userName The username to get files for
     * @return List of file paths for the user's statistics
     */
    QStringList getUserStatsFiles(const QString &userName) const;

signals:
    /**
     * @brief Signal emitted when user statistics are received.
     * 
     * @param userName The username associated with the statistics
     * @param fileName The name of the statistics file
     */
    void userStatsReceived(const QString &userName, const QString &fileName);
    
    /**
     * @brief Signal emitted when the directory status changes.
     * 
     * @param exists True if the directory exists, false otherwise
     */
    void directoryStatusChanged(bool exists);

private:
    Server *m_server; // Pointer to the server instance
    QMutex m_mutex; // Mutex for thread safety
    QDir m_statsDirectory; // The user statistics directory
    
    /**
     * @brief Ensure the user statistics directory exists.
     * 
     * Creates the directory if it doesn't exist.
     * 
     * @return True if the directory exists or was created, false otherwise
     */
    bool ensureDirectoryExists();
    
    /**
     * @brief Create user-specific subdirectory.
     * 
     * @param userName The username to create a directory for
     * @return True if the directory exists or was created, false otherwise
     */
    bool ensureUserDirectoryExists(const QString &userName);
    
    /**
     * @brief Parse and validate a CSV user statistics file.
     * 
     * @param fileData The content of the statistics file
     * @return True if the file is valid, false otherwise
     */
    bool validateStatsFile(const QByteArray &fileData);
};

#endif // MUMBLE_MURMUR_USERSTATISTICSMODULE_H_