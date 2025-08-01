// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QMutexLocker>
#include <QUuid>
#include <QDebug>

#include "DBWrapper.h"
#include "database/ConnectionParameter.h"

DBWrapper::DBWrapper(const ::mumble::db::ConnectionParameter &connectionParam, QObject *parent)
    : QObject(parent), m_isInitialized(false) {
    
    // Generate a unique connection name
    m_connectionName = QString("MumbleConnection-%1").arg(QUuid::createUuid().toString());
    
    // Create the database connection
    m_db = QSqlDatabase::addDatabase(connectionParam.driverName(), m_connectionName);
    
    // Set database parameters
    m_db.setDatabaseName(connectionParam.databaseName());
    m_db.setHostName(connectionParam.hostName());
    m_db.setPort(connectionParam.port());
    m_db.setUserName(connectionParam.userName());
    m_db.setPassword(connectionParam.password());
    
    // Apply additional options
    QMap<QString, QVariant> options = connectionParam.options();
    if (!options.isEmpty()) {
        QString optionsString;
        for (auto it = options.constBegin(); it != options.constEnd(); ++it) {
            if (!optionsString.isEmpty()) {
                optionsString += ";";
            }
            optionsString += it.key() + "=" + it.value().toString();
        }
        m_db.setConnectOptions(optionsString);
    }
}

DBWrapper::~DBWrapper() {
    QMutexLocker locker(&m_mutex);
    
    if (m_db.isOpen()) {
        m_db.close();
    }
    
    // Remove the database connection
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DBWrapper::initialize() {
    QMutexLocker locker(&m_mutex);
    
    // Already initialized
    if (m_isInitialized) {
        return true;
    }
    
    // Open the database connection
    if (!m_db.open()) {
        qWarning() << "Failed to open database connection:" << m_db.lastError().text();
        return false;
    }
    
    // Initialize database tables and indexes if needed
    // This would depend on the specific database schema
    
    m_isInitialized = true;
    return true;
}

bool DBWrapper::isOpen() const {
    QMutexLocker locker(&m_mutex);
    return m_db.isOpen();
}

QSqlQuery DBWrapper::execute(const QString &query, const QVariantList &params) {
    QMutexLocker locker(&m_mutex);
    
    QSqlQuery sqlQuery(m_db);
    sqlQuery.prepare(query);
    
    // Bind parameters
    for (const QVariant &param : params) {
        sqlQuery.addBindValue(param);
    }
    
    // Execute the query
    if (!sqlQuery.exec()) {
        qWarning() << "Query execution failed:" << sqlQuery.lastError().text();
        qWarning() << "Query was:" << query;
    }
    
    return sqlQuery;
}

QVariant DBWrapper::scalar(const QString &query, const QVariantList &params) {
    QSqlQuery sqlQuery = execute(query, params);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0);
    }
    
    return QVariant();
}

bool DBWrapper::beginTransaction() {
    QMutexLocker locker(&m_mutex);
    return m_db.transaction();
}

bool DBWrapper::commitTransaction() {
    QMutexLocker locker(&m_mutex);
    return m_db.commit();
}

bool DBWrapper::rollbackTransaction() {
    QMutexLocker locker(&m_mutex);
    return m_db.rollback();
}

QSqlError DBWrapper::lastError() const {
    QMutexLocker locker(&m_mutex);
    return m_db.lastError();
}

QMap<int, QString> DBWrapper::getUserProperties(int userID) {
    QMap<int, QString> properties;
    
    QSqlQuery query = execute("SELECT property_id, property_value FROM user_properties WHERE user_id = ?", 
                              QVariantList() << userID);
    
    while (query.next()) {
        int propertyID = query.value(0).toInt();
        QString value = query.value(1).toString();
        properties.insert(propertyID, value);
    }
    
    return properties;
}

bool DBWrapper::setUserProperties(int userID, const QMap<int, QString> &properties) {
    // Start a transaction
    if (!beginTransaction()) {
        return false;
    }
    
    try {
        // Delete existing properties
        execute("DELETE FROM user_properties WHERE user_id = ?", QVariantList() << userID);
        
        // Insert new properties
        for (auto it = properties.constBegin(); it != properties.constEnd(); ++it) {
            execute("INSERT INTO user_properties (user_id, property_id, property_value) VALUES (?, ?, ?)",
                    QVariantList() << userID << it.key() << it.value());
        }
        
        // Commit the transaction
        if (!commitTransaction()) {
            rollbackTransaction();
            return false;
        }
        
        return true;
    }
    catch (...) {
        // Rollback on any error
        rollbackTransaction();
        return false;
    }
}

bool DBWrapper::storeTexture(int userID, const QByteArray &texture) {
    // Check if texture already exists
    QSqlQuery query = execute("SELECT texture_id FROM textures WHERE user_id = ?", 
                              QVariantList() << userID);
    
    if (query.next()) {
        // Update existing texture
        return execute("UPDATE textures SET texture_data = ? WHERE user_id = ?",
                      QVariantList() << texture << userID).numRowsAffected() > 0;
    }
    else {
        // Insert new texture
        return execute("INSERT INTO textures (user_id, texture_data) VALUES (?, ?)",
                      QVariantList() << userID << texture).numRowsAffected() > 0;
    }
}

QByteArray DBWrapper::getTexture(int userID) {
    QSqlQuery query = execute("SELECT texture_data FROM textures WHERE user_id = ?", 
                              QVariantList() << userID);
    
    if (query.next()) {
        return query.value(0).toByteArray();
    }
    
    return QByteArray();
}