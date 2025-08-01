// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_USERDATAMODULE_H_
#define MUMBLE_MURMUR_USERDATAMODULE_H_

#include "IServerModule.h"
#include "../User.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtNetwork/QSslCertificate>

class Server;
class ServerUser;
class Channel;

/**
 * @brief The UserDataModule class manages user data operations.
 * 
 * This module handles user authentication, registration, and profile management.
 * It encapsulates all user-related data operations that were previously
 * integrated into the Server class.
 */
class UserDataModule : public IServerModule {
    Q_OBJECT
public:
    /**
     * @brief Constructor for UserDataModule.
     * @param parent The parent QObject
     */
    explicit UserDataModule(QObject *parent = nullptr);
    
    /**
     * @brief Destructor for UserDataModule.
     */
    ~UserDataModule();
    
    // IServerModule interface implementation
    bool initialize(Server *server) override;
    QString name() const override;
    QString version() const override;
    QString description() const override;
    QVariant getSetting(const QString &key, const QVariant &defaultValue = QVariant()) const override;
    bool setSetting(const QString &key, const QVariant &value) override;
    void shutdown() override;
    
    /**
     * @brief Authenticate a user.
     * 
     * @param name Username to authenticate
     * @param password Password for authentication
     * @param sessionId Session ID for the connection
     * @param emails List of email addresses for the user
     * @param certhash Certificate hash
     * @param bStrongCert Whether the certificate is strong
     * @param certs List of certificates
     * @return User ID of authenticated user, negative values for errors
     */
    int authenticate(QString &name, const QString &password, int sessionId = 0, 
                    const QStringList &emails = {}, const QString &certhash = {},
                    bool bStrongCert = false, const QList<QSslCertificate> &certs = {});
    
    /**
     * @brief Register a new user.
     * 
     * @param user The server user to register
     * @return True if registration was successful, false otherwise
     */
    bool registerUser(ServerUser &user);
    
    /**
     * @brief Register a new user with the provided information.
     * 
     * @param userInfo The user information
     * @return User ID of the registered user, or -1 on failure
     */
    int registerUser(const ServerUserInfo &userInfo);
    
    /**
     * @brief Unregister a user.
     * 
     * @param id User ID to unregister
     * @return True if unregistration was successful, false otherwise
     */
    bool unregisterUser(int id);
    
    /**
     * @brief Get the name of a registered user.
     * 
     * @param userID User ID to look up
     * @return The username, or an empty string if not found
     */
    QString getRegisteredUserName(int userID);
    
    /**
     * @brief Get the ID of a registered user by name.
     * 
     * @param name Username to look up
     * @return The user ID, or -1 if not found
     */
    int getRegisteredUserID(const QString &name);
    
    /**
     * @brief Set user properties.
     * 
     * @param userID User ID to update
     * @param properties Map of property IDs to values
     * @return True if properties were set successfully, false otherwise
     */
    bool setUserProperties(int userID, QMap<int, QString> properties);
    
    /**
     * @brief Get user properties.
     * 
     * @param userID User ID to retrieve properties for
     * @return Map of property IDs to values
     */
    QMap<int, QString> getUserProperties(int userID);
    
    /**
     * @brief Get properties for all registered users.
     * 
     * @param nameSubstring Optional substring to filter usernames
     * @return Vector of user information objects
     */
    std::vector<UserInfo> getAllRegisteredUserProperties(QString nameSubstring = "");
    
    /**
     * @brief Set user comment.
     * 
     * @param user The server user to update
     * @param comment The new comment
     * @return True if comment was set successfully, false otherwise
     */
    bool setComment(ServerUser &user, const QString &comment);
    
    /**
     * @brief Load user comment.
     * 
     * @param user The server user to load comment for
     */
    void loadComment(ServerUser &user);
    
    /**
     * @brief Set user texture.
     * 
     * @param user The server user to update
     * @param texture The new texture
     * @return True if texture was set successfully, false otherwise
     */
    bool setTexture(ServerUser &user, const QByteArray &texture);
    
    /**
     * @brief Store user texture.
     * 
     * @param userInfo The user information
     * @param texture The texture to store
     * @return True if texture was stored successfully, false otherwise
     */
    bool storeTexture(const ServerUserInfo &userInfo, const QByteArray &texture);
    
    /**
     * @brief Load user texture.
     * 
     * @param user The server user to load texture for
     */
    void loadTexture(ServerUser &user);
    
    /**
     * @brief Get user texture.
     * 
     * @param userID User ID to retrieve texture for
     * @return The texture data
     */
    QByteArray getTexture(int userID);
    
    /**
     * @brief Check if a user ID is valid.
     * 
     * @param userID User ID to check
     * @return True if the user ID is valid, false otherwise
     */
    bool isValidUserID(int userID);
    
    /**
     * @brief Set temporary groups for a user.
     * 
     * @param userid User ID
     * @param sessionId Session ID
     * @param cChannel Channel for the groups
     * @param groups List of group names
     */
    void setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups);
    
    /**
     * @brief Clear temporary groups for a user.
     * 
     * @param user User to clear groups for
     * @param cChannel Channel to clear groups in, or nullptr for all channels
     * @param recurse Whether to recursively clear groups in subchannels
     */
    void clearTempGroups(User *user, Channel *cChannel = nullptr, bool recurse = true);

signals:
    /**
     * @brief Signal emitted when a user is authenticated.
     * 
     * @param userID User ID of the authenticated user
     * @param username Username of the authenticated user
     */
    void userAuthenticated(int userID, const QString &username);
    
    /**
     * @brief Signal emitted when a user is registered.
     * 
     * @param userID User ID of the registered user
     * @param username Username of the registered user
     */
    void userRegistered(int userID, const QString &username);
    
    /**
     * @brief Signal emitted when a user is unregistered.
     * 
     * @param userID User ID of the unregistered user
     */
    void userUnregistered(int userID);
    
    /**
     * @brief Signal emitted when user properties are changed.
     * 
     * @param userID User ID of the updated user
     * @param properties Map of property IDs to values
     */
    void userPropertiesChanged(int userID, const QMap<int, QString> &properties);

private:
    Server *m_server; // Pointer to the server instance
    QRecursiveMutex m_mutex; // Mutex for thread safety
    
    // Cache for user data
    QHash<int, QString> m_userNameCache;
    QHash<QString, int> m_userIDCache;
    QHash<int, QMap<int, QString>> m_userPropertiesCache;
};

#endif // MUMBLE_MURMUR_USERDATAMODULE_H_