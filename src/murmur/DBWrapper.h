// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_DBWRAPPER_H_
#define MUMBLE_MURMUR_DBWRAPPER_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QMutex>

namespace mumble {
namespace db {
class ConnectionParameter;
}
}

/**
 * @brief The DBWrapper class provides a wrapper for database operations.
 * 
 * This class abstracts database operations and provides a unified
 * interface for interacting with the database, regardless of the
 * specific database backend being used.
 */
class DBWrapper : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for DBWrapper
     * 
     * @param connectionParam The database connection parameters
     * @param parent The parent QObject
     */
    explicit DBWrapper(const ::mumble::db::ConnectionParameter &connectionParam, QObject *parent = nullptr);
    
    /**
     * @brief Destructor for DBWrapper
     */
    ~DBWrapper();
    
    /**
     * @brief Initialize the database connection
     * 
     * @return true if initialization was successful
     */
    bool initialize();
    
    /**
     * @brief Check if the database connection is open
     * 
     * @return true if the connection is open
     */
    bool isOpen() const;
    
    /**
     * @brief Execute a SQL query
     * 
     * @param query The SQL query to execute
     * @param params The parameters for the query
     * @return QSqlQuery object with the results
     */
    QSqlQuery execute(const QString &query, const QVariantList &params = QVariantList());
    
    /**
     * @brief Execute a SQL query and get the first result
     * 
     * @param query The SQL query to execute
     * @param params The parameters for the query
     * @return QVariant with the first result
     */
    QVariant scalar(const QString &query, const QVariantList &params = QVariantList());
    
    /**
     * @brief Begin a transaction
     * 
     * @return true if the transaction was started successfully
     */
    bool beginTransaction();
    
    /**
     * @brief Commit a transaction
     * 
     * @return true if the transaction was committed successfully
     */
    bool commitTransaction();
    
    /**
     * @brief Rollback a transaction
     * 
     * @return true if the transaction was rolled back successfully
     */
    bool rollbackTransaction();
    
    /**
     * @brief Get the last error that occurred
     * 
     * @return QSqlError object containing the error
     */
    QSqlError lastError() const;
    
    /**
     * @brief Get user properties from the database
     * 
     * @param userID The ID of the user
     * @return QMap containing the user properties
     */
    QMap<int, QString> getUserProperties(int userID);
    
    /**
     * @brief Set user properties in the database
     * 
     * @param userID The ID of the user
     * @param properties The properties to set
     * @return true if the properties were set successfully
     */
    bool setUserProperties(int userID, const QMap<int, QString> &properties);
    
    /**
     * @brief Store user texture in the database
     * 
     * @param userID The ID of the user
     * @param texture The texture data
     * @return true if the texture was stored successfully
     */
    bool storeTexture(int userID, const QByteArray &texture);
    
    /**
     * @brief Get user texture from the database
     * 
     * @param userID The ID of the user
     * @return QByteArray containing the texture data
     */
    QByteArray getTexture(int userID);
    
private:
    QString m_connectionName; // Name of the database connection
    QSqlDatabase m_db; // Database connection
    mutable QMutex m_mutex; // Mutex for thread safety
    bool m_isInitialized; // Flag indicating if the database is initialized
};

#endif // MUMBLE_MURMUR_DBWRAPPER_H_