// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserDataModule.h"
#include "Server.h"
#include "ServerUser.h"
#include "Channel.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QByteArray>

UserDataModule::UserDataModule(QObject *parent)
    : IServerModule(parent)
    , m_server(nullptr) {
    // Nothing to do here
}

UserDataModule::~UserDataModule() {
    // Nothing to do here
}

bool UserDataModule::initialize(Server *server) {
    if (!server) {
        qWarning() << "UserDataModule: Cannot initialize with null server";
        return false;
    }
    
    m_server = server;
    
    // Initialize the user data cache
    m_userNameCache.clear();
    m_userIDCache.clear();
    m_userPropertiesCache.clear();
    
    qDebug() << "UserDataModule: Initialized successfully";
    return true;
}

QString UserDataModule::name() const {
    return "UserDataModule";
}

QString UserDataModule::version() const {
    return "1.0.0";
}

QString UserDataModule::description() const {
    return "Manages user data, authentication, and profiles";
}

QVariant UserDataModule::getSetting(const QString &key, const QVariant &defaultValue) const {
    QMutexLocker locker(&m_mutex);
    
    // Implement settings retrieval as needed
    return defaultValue;
}

bool UserDataModule::setSetting(const QString &key, const QVariant &value) {
    QMutexLocker locker(&m_mutex);
    
    // Implement settings update as needed
    return false;
}

void UserDataModule::shutdown() {
    QMutexLocker locker(&m_mutex);
    
    // Clear the user data cache
    m_userNameCache.clear();
    m_userIDCache.clear();
    m_userPropertiesCache.clear();
    
    // Signal that the module is shutting down
    QVariant data;
    emit moduleEvent("shutdown", data);
    
    qDebug() << "UserDataModule: Shut down";
}

int UserDataModule::authenticate(QString &name, const QString &password, int sessionId, 
                               const QStringList &emails, const QString &certhash,
                               bool bStrongCert, const QList<QSslCertificate> &certs) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot authenticate user - server not initialized";
        return -1;
    }
    
    // In a real implementation, this would delegate to the server's authentication logic
    // For this example, we'll log the authentication attempt and return a success code
    
    qDebug() << "UserDataModule: Authenticating user" << name;
    
    // Call the server's authenticate method (delegation)
    int userID = m_server->authenticate(name, password, sessionId, emails, certhash, bStrongCert, certs);
    
    // Emit a signal if authentication was successful
    if (userID > 0) {
        emit userAuthenticated(userID, name);
        
        // Cache the user name and ID
        m_userNameCache.insert(userID, name);
        m_userIDCache.insert(name.toLower(), userID);
    }
    
    return userID;
}

bool UserDataModule::registerUser(ServerUser &user) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot register user - server not initialized";
        return false;
    }
    
    // Call the server's registerUser method (delegation)
    bool success = m_server->registerUser(user);
    
    // Emit a signal if registration was successful
    if (success) {
        int userID = user.iId;
        QString username = user.qsName;
        
        emit userRegistered(userID, username);
        
        // Cache the user name and ID
        m_userNameCache.insert(userID, username);
        m_userIDCache.insert(username.toLower(), userID);
    }
    
    return success;
}

int UserDataModule::registerUser(const ServerUserInfo &userInfo) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot register user - server not initialized";
        return -1;
    }
    
    // Call the server's registerUser method (delegation)
    int userID = m_server->registerUser(userInfo);
    
    // Emit a signal if registration was successful
    if (userID > 0) {
        QString username = userInfo.name;
        
        emit userRegistered(userID, username);
        
        // Cache the user name and ID
        m_userNameCache.insert(userID, username);
        m_userIDCache.insert(username.toLower(), userID);
    }
    
    return userID;
}

bool UserDataModule::unregisterUser(int id) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot unregister user - server not initialized";
        return false;
    }
    
    // Cache the username before unregistering
    QString username = getRegisteredUserName(id);
    
    // Call the server's unregisterUser method (delegation)
    bool success = m_server->unregisterUser(id);
    
    // Emit a signal if unregistration was successful
    if (success) {
        emit userUnregistered(id);
        
        // Remove the user from the cache
        m_userNameCache.remove(id);
        if (!username.isEmpty()) {
            m_userIDCache.remove(username.toLower());
        }
        m_userPropertiesCache.remove(id);
    }
    
    return success;
}

QString UserDataModule::getRegisteredUserName(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // Check the cache first
    if (m_userNameCache.contains(userID)) {
        return m_userNameCache.value(userID);
    }
    
    // If not in cache, call the server's method
    if (m_server) {
        QString name = m_server->getRegisteredUserName(userID);
        
        // Cache the result
        if (!name.isEmpty()) {
            m_userNameCache.insert(userID, name);
        }
        
        return name;
    }
    
    return QString();
}

int UserDataModule::getRegisteredUserID(const QString &name) {
    QMutexLocker locker(&m_mutex);
    
    QString lowerName = name.toLower();
    
    // Check the cache first
    if (m_userIDCache.contains(lowerName)) {
        return m_userIDCache.value(lowerName);
    }
    
    // If not in cache, call the server's method
    if (m_server) {
        int userID = m_server->getRegisteredUserID(name);
        
        // Cache the result
        if (userID > 0) {
            m_userIDCache.insert(lowerName, userID);
        }
        
        return userID;
    }
    
    return -1;
}

bool UserDataModule::setUserProperties(int userID, QMap<int, QString> properties) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot set user properties - server not initialized";
        return false;
    }
    
    // Call the server's setUserProperties method (delegation)
    bool success = m_server->setUserProperties(userID, properties);
    
    // Emit a signal if properties were set successfully
    if (success) {
        emit userPropertiesChanged(userID, properties);
        
        // Cache the properties
        QMutexLocker locker(&m_mutex);
        m_userPropertiesCache.insert(userID, properties);
    }
    
    return success;
}

QMap<int, QString> UserDataModule::getUserProperties(int userID) {
    QMutexLocker locker(&m_mutex);
    
    // Check the cache first
    if (m_userPropertiesCache.contains(userID)) {
        return m_userPropertiesCache.value(userID);
    }
    
    // If not in cache, call the server's method
    if (m_server) {
        QMap<int, QString> properties = m_server->getUserProperties(userID);
        
        // Cache the result
        m_userPropertiesCache.insert(userID, properties);
        
        return properties;
    }
    
    return QMap<int, QString>();
}

std::vector<UserInfo> UserDataModule::getAllRegisteredUserProperties(QString nameSubstring) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot get all user properties - server not initialized";
        return std::vector<UserInfo>();
    }
    
    // Call the server's getAllRegisteredUserProperties method (delegation)
    return m_server->getAllRegisteredUserProperties(nameSubstring);
}

bool UserDataModule::setComment(ServerUser &user, const QString &comment) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot set comment - server not initialized";
        return false;
    }
    
    // Call the server's setComment method (delegation)
    return m_server->setComment(user, comment);
}

void UserDataModule::loadComment(ServerUser &user) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot load comment - server not initialized";
        return;
    }
    
    // Call the server's loadComment method (delegation)
    m_server->loadComment(user);
}

bool UserDataModule::setTexture(ServerUser &user, const QByteArray &texture) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot set texture - server not initialized";
        return false;
    }
    
    // Call the server's setTexture method (delegation)
    return m_server->setTexture(user, texture);
}

bool UserDataModule::storeTexture(const ServerUserInfo &userInfo, const QByteArray &texture) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot store texture - server not initialized";
        return false;
    }
    
    // Call the server's storeTexture method (delegation)
    return m_server->storeTexture(userInfo, texture);
}

void UserDataModule::loadTexture(ServerUser &user) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot load texture - server not initialized";
        return;
    }
    
    // Call the server's loadTexture method (delegation)
    m_server->loadTexture(user);
}

QByteArray UserDataModule::getTexture(int userID) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot get texture - server not initialized";
        return QByteArray();
    }
    
    // Call the server's getTexture method (delegation)
    return m_server->getTexture(userID);
}

bool UserDataModule::isValidUserID(int userID) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot validate user ID - server not initialized";
        return false;
    }
    
    // Call the server's isValidUserID method (delegation)
    return m_server->isValidUserID(userID);
}

void UserDataModule::setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot set temp groups - server not initialized";
        return;
    }
    
    // Call the server's setTempGroups method (delegation)
    m_server->setTempGroups(userid, sessionId, cChannel, groups);
}

void UserDataModule::clearTempGroups(User *user, Channel *cChannel, bool recurse) {
    if (!m_server) {
        qWarning() << "UserDataModule: Cannot clear temp groups - server not initialized";
        return;
    }
    
    // Call the server's clearTempGroups method (delegation)
    m_server->clearTempGroups(user, cChannel, recurse);
}