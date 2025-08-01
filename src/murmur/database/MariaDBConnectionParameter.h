// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_MARIADBCONNECTIONPARAMETER_H_
#define MUMBLE_DATABASE_MARIADBCONNECTIONPARAMETER_H_

#include <QString>
#include <QVariant>
#include <QMap>
#include <QRegExp>

namespace mumble {
namespace db {

// Forward declaration
class ConnectionParameter;

/**
 * @brief Connection parameters for MariaDB
 *
 * This class represents connection parameters for MariaDB.
 */
class MariaDBConnectionParameter {
public:
    /**
     * @brief Default constructor
     */
    MariaDBConnectionParameter() 
        : m_port(3306) {
    }

    /**
     * @brief Constructor with database name
     *
     * @param databaseName The name of the database
     */
    MariaDBConnectionParameter(const QString &databaseName)
        : m_port(3306), m_databaseName(databaseName) {
        // Set public fields
        m_dbName = databaseName;
    }

    /**
     * @brief Constructor with detailed connection parameters
     *
     * @param hostName The host name
     * @param port The port number
     * @param databaseName The database name
     * @param userName The user name
     * @param password The password
     */
    MariaDBConnectionParameter(const QString &hostName, int port,
                              const QString &databaseName,
                              const QString &userName,
                              const QString &password)
        : m_hostName(hostName), 
          m_port(port),
          m_databaseName(databaseName),
          m_userName(userName),
          m_password(password) {
        // Set public fields
        m_host = hostName;
        m_portStr = QString::number(port);
        m_dbName = databaseName;
        m_userNameStr = userName;
        m_passwordStr = password;
    }

    /**
     * @brief Get the database driver name
     * 
     * @return Database driver name
     */
    QString driverName() const {
        return "QMYSQL";
    }
    
    /**
     * @brief Get the database name
     * 
     * @return Database name
     */
    QString databaseName() const {
        if (!m_databaseName.isEmpty()) {
            return m_databaseName;
        }
        return m_dbName;
    }
    
    /**
     * @brief Get the connection options
     * 
     * @return Connection options as a map
     */
    QMap<QString, QVariant> options() const {
        QMap<QString, QVariant> opts;
        opts["MYSQL_OPT_RECONNECT"] = true;
        return opts;
    }
    
    /**
     * @brief Get the host name
     * 
     * @return Host name
     */
    QString hostName() const {
        if (!m_hostName.isEmpty()) {
            return m_hostName;
        }
        return m_host;
    }
    
    /**
     * @brief Get the port
     * 
     * @return Port number
     */
    int port() const {
        // Try to convert port string to int
        bool ok = false;
        if (!m_portStr.isEmpty()) {
            int portNum = m_portStr.toInt(&ok);
            if (ok) {
                return portNum;
            }
        }
        return m_port;
    }
    
    /**
     * @brief Get the user name
     * 
     * @return User name
     */
    QString userName() const {
        if (!m_userName.isEmpty()) {
            return m_userName;
        }
        return m_userNameStr;
    }
    
    /**
     * @brief Get the password
     * 
     * @return Password
     */
    QString password() const {
        if (!m_password.isEmpty()) {
            return m_password;
        }
        return m_passwordStr;
    }
    
    /**
     * @brief Check if the connection parameters are valid
     * 
     * @return true if the parameters are valid
     */
    bool isValid() const {
        return !databaseName().isEmpty();
    }
    
    /**
     * @brief Set the port number
     *
     * @param portNum The port number
     */
    void setPort(int portNum) {
        m_port = portNum;
        m_portStr = QString::number(portNum);
    }

    /**
     * @brief Set the port from a string
     *
     * @param portStr The port as a string
     */
    void setPortFromString(const QString &portStr) {
        m_portStr = portStr;
        bool ok = false;
        int portNum = portStr.toInt(&ok);
        if (ok) {
            m_port = portNum;
        }
    }

    /**
     * @brief Set the user name
     *
     * @param userNameVal The user name
     */
    void setUserName(const QString &userNameVal) {
        m_userName = userNameVal;
        m_userNameStr = userNameVal;
    }

    /**
     * @brief Set the password
     *
     * @param passwordVal The password
     */
    void setPassword(const QString &passwordVal) {
        m_password = passwordVal;
        m_passwordStr = passwordVal;
    }

    /**
     * @brief Set the host name
     *
     * @param hostNameVal The host name
     */
    void setHostName(const QString &hostNameVal) {
        m_hostName = hostNameVal;
        m_host = hostNameVal;
    }

    /**
     * @brief Set the database name
     *
     * @param dbNameVal The database name
     */
    void setDatabaseName(const QString &dbNameVal) {
        m_databaseName = dbNameVal;
        m_dbName = dbNameVal;
    }

    /**
     * @brief Create a ConnectionParameter from this MariaDBConnectionParameter
     * 
     * This allows us to convert to the type expected by Server without
     * direct inheritance, avoiding the incomplete type issues.
     * 
     * @return ConnectionParameter* A pointer to a new ConnectionParameter
     */
    ConnectionParameter* toConnectionParameter() const;

    // Public members for direct access in main.cpp
    QString m_host;             ///< Host name
    QString m_portStr;          ///< Port number as string
    QString m_dbName;           ///< Database name
    QString m_userNameStr;      ///< User name
    QString m_passwordStr;      ///< Password

private:
    QString m_hostName;         ///< Host name (private)
    int m_port;                 ///< Port number (private)
    QString m_databaseName;     ///< Database name (private)
    QString m_userName;         ///< User name (private)
    QString m_password;         ///< Password (private)
};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_MARIADBCONNECTIONPARAMETER_H_