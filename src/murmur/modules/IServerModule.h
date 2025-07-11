// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ISERVERMODULE_H_
#define MUMBLE_MURMUR_ISERVERMODULE_H_

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class Server;

/**
 * @brief The IServerModule class defines the interface for all server modules.
 * 
 * This interface allows for the creation of modular components that can be
 * dynamically loaded and integrated with the server. Modules provide specific
 * functionality such as user data management, propagation simulation, etc.
 */
class IServerModule : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for IServerModule.
     * @param parent The parent QObject
     */
    explicit IServerModule(QObject *parent = nullptr) : QObject(parent), m_server(nullptr) {}
    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~IServerModule() {}
    
    /**
     * @brief Initialize the module.
     * 
     * This method is called during server startup to initialize the module.
     * It should perform any necessary setup operations.
     * 
     * @param server Pointer to the Server instance
     * @return True if initialization was successful, false otherwise
     */
    virtual bool initialize(Server *server) = 0;
    
    /**
     * @brief Get the name of the module.
     * 
     * @return The module name
     */
    virtual QString name() const = 0;
    
    /**
     * @brief Get the version of the module.
     * 
     * @return The module version
     */
    virtual QString version() const = 0;
    
    /**
     * @brief Get the description of the module.
     * 
     * @return The module description
     */
    virtual QString description() const = 0;
    
    /**
     * @brief Get a module-specific setting or parameter.
     * 
     * This method allows accessing module-specific settings or internal state.
     * 
     * @param key The setting key
     * @param defaultValue The default value if the key is not found
     * @return The setting value or the default value if not found
     */
    virtual QVariant getSetting(const QString &key, const QVariant &defaultValue = QVariant()) const = 0;
    
    /**
     * @brief Set a module-specific setting or parameter.
     * 
     * This method allows changing module-specific settings or internal state.
     * 
     * @param key The setting key
     * @param value The new value
     * @return True if the setting was successfully changed, false otherwise
     */
    virtual bool setSetting(const QString &key, const QVariant &value) = 0;
    
    /**
     * @brief Shutdown the module.
     * 
     * This method is called during server shutdown to clean up resources.
     */
    virtual void shutdown() = 0;

signals:
    /**
     * @brief Signal emitted when a module-specific event occurs.
     * 
     * This allows modules to communicate events to the server or other modules.
     * 
     * @param eventName The name of the event
     * @param data The event data
     */
    void moduleEvent(const QString &eventName, const QVariant &data);

protected:
    Server *m_server; // Pointer to the server instance
};

#endif // MUMBLE_MURMUR_ISERVERMODULE_H_