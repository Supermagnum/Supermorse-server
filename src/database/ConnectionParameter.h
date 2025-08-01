// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_CONNECTIONPARAMETER_H_
#define MUMBLE_DATABASE_CONNECTIONPARAMETER_H_

#include <QString>
#include <QVariant>
#include <QMap>

namespace mumble {
namespace db {

/**
 * @brief The ConnectionParameter class defines database connection parameters.
 * 
 * This is the base class for all database connection parameters.
 * Specific database backends implement this interface.
 */
class ConnectionParameter {
public:
    /**
     * @brief Constructor
     */
    ConnectionParameter() = default;
    
    /**
     * @brief Virtual destructor
     */
    virtual ~ConnectionParameter() = default;
    
    /**
     * @brief Get the database driver name
     * 
     * @return Database driver name
     */
    virtual QString driverName() const = 0;
    
    /**
     * @brief Get the database name
     * 
     * @return Database name
     */
    virtual QString databaseName() const = 0;
    
    /**
     * @brief Get the connection options
     * 
     * @return Connection options as a map
     */
    virtual QMap<QString, QVariant> options() const = 0;
    
    /**
     * @brief Get the host name
     * 
     * @return Host name
     */
    virtual QString hostName() const = 0;
    
    /**
     * @brief Get the port
     * 
     * @return Port number
     */
    virtual int port() const = 0;
    
    /**
     * @brief Get the user name
     * 
     * @return User name
     */
    virtual QString userName() const = 0;
    
    /**
     * @brief Get the password
     * 
     * @return Password
     */
    virtual QString password() const = 0;
    
    /**
     * @brief Check if the connection parameters are valid
     * 
     * @return true if the parameters are valid
     */
    virtual bool isValid() const = 0;
    
    /**
     * @brief Clone the connection parameters
     * 
     * @return Pointer to a new instance of the connection parameters
     */
    virtual ConnectionParameter* clone() const = 0;
    
    /**
     * @brief Get a string representation of the connection
     * 
     * @return String representation
     */
    virtual QString toString() const {
        return QString("%1://%2@%3:%4/%5")
            .arg(driverName())
            .arg(userName())
            .arg(hostName())
            .arg(port())
            .arg(databaseName());
    }
    
    /**
     * @brief Get a safe string representation (without password)
     * 
     * @return Safe string representation
     */
    virtual QString toSafeString() const {
        return QString("%1://%2@%3:%4/%5")
            .arg(driverName())
            .arg(userName())
            .arg(hostName())
            .arg(port())
            .arg(databaseName());
    }
    
    /**
     * @brief Create a database connection parameters from a connection string
     * 
     * @param connectionString Connection string
     * @return ConnectionParameter* Pointer to the connection parameters
     */
    static ConnectionParameter* fromConnectionString(const QString &connectionString);
};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_CONNECTIONPARAMETER_H_