// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserDataModule.h"
#include "../Server.h"

#include <QtCore/QDebug>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>

UserDataModule::UserDataModule(QObject *parent)
    : IServerModule(parent)
    , m_server(nullptr) {
    // Nothing to do here
}

UserDataModule::~UserDataModule() {
    // Clear caches
    m_userNameCache.clear();
    m_userIDCache.clear();
    m_userPropertiesCache.clear();
}

bool UserDataModule::initialize(Server *server) {
    if (!server) {
        qWarning() << "UserDataModule: Cannot initialize with null server";
        return false;
    }
    
    m_server = server;
    
    qDebug() << "UserDataModule: Initialized";
    
    return true;
}

QString UserDataModule::name() const {
    return "UserDataModule";
}

QString UserDataModule::version() const {
    return "1.0.0";
}

QString UserDataModule::description() const {
    return "Manages user authentication, registration, and profile data";
}

QVariant UserDataModule::getSetting(const QString &key, const QVariant &defaultValue) const {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would read from a settings store
    // For this simplified version, we just return the default value
    return defaultValue;
}

bool UserDataModule::setSetting(const QString &key, const QVariant &value) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would write to a settings store
    // For this simplified version, we just log the change
    qDebug() << "UserDataModule: Setting" << key << "to" << value;
    
    return true;
}

void UserDataModule::shutdown() {
    // Clear caches
    m_userNameCache.clear();
    m_userIDCache.clear();
    m_userPropertiesCache.clear();
    
    qDebug() << "UserDataModule: Shutdown";
}

int UserDataModule::authenticate(QString &name, const QString &password, int sessionId,
                              const QStringList &emails, const QString &certhash,
                              bool bStrongCert, const QList<QSslCertificate> &certs) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would query the database
    // and perform authentication
    
    // For this simplified version, we just check if the user is registered
    int userID = getRegisteredUserID(name);
    
    if (userID > 0) {
        // User is registered, emit authentication signal
        emit userAuthenticated(userID, name);
        return userID;
    } else {
        // User is not registered, allow anonymous authentication
        return -2; // -2 indicates unknown user (fallthrough)
    }
}

bool UserDataModule::registerUser(ServerUser &user) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user is already registered
    if (user.iId > 0) {
        return true; // Already registered
    }
    
    // Create user info for registration
    ServerUserInfo userInfo;
    userInfo.name = user.qsName;
    // Set other user info fields as needed
    
    // Register the user
    int userID = registerUser(userInfo);
    
    if (userID > 0) {
        // Registration successful, update user ID
        user.iId = userID;
        return true;
    } else {
        // Registration failed
        return false;
    }
}

int UserDataModule::registerUser(const ServerUserInfo &userInfo) {
    QMutexLocker locker(&m_mutex);
    
    // Check if name is already registered
    if (getRegisteredUserID(userInfo.name) > 0) {
        qWarning() << "UserDataModule: Cannot register user, name already in use:" << userInfo.name;
        return -1;
    }
    
    // In a real implementation, this would insert into the database
    // and return the new user ID
    
    // For this simplified version, we just generate a random ID
    int userID = qrand() % 10000 + 1; // Random ID between 1 and 10000
    
    // Add to caches
    m_userNameCache.insert(userID, userInfo.name);
    m_userIDCache.insert(userInfo.name, userID);
    
    // Create initial properties
    QMap<int, QString> properties;
    properties.insert(0, userInfo.name);
    // Add other properties as needed
    
    m_userPropertiesCache.insert(userID, properties);
    
    // Emit signal
    emit userRegistered(userID, userInfo.name);
    
    qDebug() << "UserDataModule: Registered user" << userInfo.name << "with ID" << userID;
    
    return userID;
}

bool UserDataModule::unregisterUser(int id) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user exists
    if (!m_userNameCache.contains(id)) {
        qWarning() << "UserDataModule: Cannot unregister user, ID not found:" << id;
        return false;
    }
    
    // Get the username
    QString name = m_userNameCache.value(id);
    
    // Remove from caches
    m_userNameCache.remove(id);
    m_userIDCache.remove(name);
    m_userPropertiesCache.remove(id);
    
    // Emit signal
    emit userUnregistered(id);
    
    qDebug() << "UserDataModule: Unregistered user" << name << "with ID" << id;
    
    return true;
}

QString UserDataModule::getRegisteredUserName(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // Return the username from cache
    return m_userNameCache.value(userID, QString());
}

int UserDataModule::getRegisteredUserID(const QString &name) {
    QMutexLocker locker(&m_mutex);
    
    // Return the user ID from cache
    return m_userIDCache.value(name, -1);
}

bool UserDataModule::setUserProperties(int userID, QMap<int, QString> properties) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user exists
    if (!m_userNameCache.contains(userID)) {
        qWarning() << "UserDataModule: Cannot set properties, user ID not found:" << userID;
        return false;
    }
    
    // Update properties in cache
    m_userPropertiesCache.insert(userID, properties);
    
    // Emit signal
    emit userPropertiesChanged(userID, properties);
    
    qDebug() << "UserDataModule: Updated properties for user ID" << userID;
    
    return true;
}

QMap<int, QString> UserDataModule::getUserProperties(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // Return the properties from cache
    return m_userPropertiesCache.value(userID, QMap<int, QString>());
}

std::vector<UserInfo> UserDataModule::getAllRegisteredUserProperties(QString nameSubstring) {
    QMutexLocker locker(&m_mutex);
    
    std::vector<UserInfo> result;
    
    // Iterate over all users
    foreach (int userID, m_userNameCache.keys()) {
        QString name = m_userNameCache.value(userID);
        
        // Skip if name doesn't match the substring filter
        if (!nameSubstring.isEmpty() && !name.contains(nameSubstring, Qt::CaseInsensitive)) {
            continue;
        }
        
        // Create user info
        UserInfo info;
        info.userID = userID;
        info.name = name;
        
        // Add properties
        if (m_userPropertiesCache.contains(userID)) {
            const QMap<int, QString> &props = m_userPropertiesCache.value(userID);
            for (auto it = props.constBegin(); it != props.constEnd(); ++it) {
                info.properties[it.key()] = it.value();
            }
        }
        
        result.push_back(info);
    }
    
    return result;
}

bool UserDataModule::setComment(ServerUser &user, const QString &comment) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user is registered
    if (user.iId <= 0) {
        qWarning() << "UserDataModule: Cannot set comment, user not registered:" << user.qsName;
        return false;
    }
    
    // Get current properties
    QMap<int, QString> properties = getUserProperties(user.iId);
    
    // Set comment property (using property ID 1 for comments)
    properties.insert(1, comment);
    
    // Update properties
    bool success = setUserProperties(user.iId, properties);
    
    if (success) {
        // Update user's comment
        user.qsComment = comment;
    }
    
    return success;
}

void UserDataModule::loadComment(ServerUser &user) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user is registered
    if (user.iId <= 0) {
        return;
    }
    
    // Get properties
    QMap<int, QString> properties = getUserProperties(user.iId);
    
    // Get comment (using property ID 1 for comments)
    QString comment = properties.value(1, QString());
    
    // Set user's comment
    user.qsComment = comment;
}

bool UserDataModule::setTexture(ServerUser &user, const QByteArray &texture) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user is registered
    if (user.iId <= 0) {
        qWarning() << "UserDataModule: Cannot set texture, user not registered:" << user.qsName;
        return false;
    }
    
    // Create user info for texture storage
    ServerUserInfo userInfo;
    userInfo.userID = user.iId;
    userInfo.name = user.qsName;
    
    // Store the texture
    bool success = storeTexture(userInfo, texture);
    
    if (success) {
        // Update user's texture
        user.qbaTexture = texture;
    }
    
    return success;
}

bool UserDataModule::storeTexture(const ServerUserInfo &userInfo, const QByteArray &texture) {
    // In a real implementation, this would store the texture in the database
    
    // For this simplified version, we just check if the user exists
    if (!m_userNameCache.contains(userInfo.userID)) {
        qWarning() << "UserDataModule: Cannot store texture, user ID not found:" << userInfo.userID;
        return false;
    }
    
    qDebug() << "UserDataModule: Stored texture for user ID" << userInfo.userID
             << ", size:" << texture.size() << "bytes";
    
    return true;
}

void UserDataModule::loadTexture(ServerUser &user) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user is registered
    if (user.iId <= 0) {
        return;
    }
    
    // Get texture
    QByteArray texture = getTexture(user.iId);
    
    // Set user's texture
    user.qbaTexture = texture;
}

QByteArray UserDataModule::getTexture(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would retrieve the texture from the database
    
    // For this simplified version, we just return an empty texture
    return QByteArray();
}

bool UserDataModule::isValidUserID(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // Check if user exists in cache
    return m_userNameCache.contains(userID);
}

void UserDataModule::setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups) {
    // In a real implementation, this would set temporary groups for the user
    
    qDebug() << "UserDataModule: Set temporary groups for user ID" << userid
             << "in channel" << (cChannel ? cChannel->qsName : "null")
             << ":" << groups.join(", ");
}

void UserDataModule::clearTempGroups(User *user, Channel *cChannel, bool recurse) {
    // In a real implementation, this would clear temporary groups for the user
    
    qDebug() << "UserDataModule: Cleared temporary groups for user"
             << (user ? user->qsName : "null")
             << "in channel" << (cChannel ? cChannel->qsName : "all channels")
             << (recurse ? "(recursive)" : "");
}