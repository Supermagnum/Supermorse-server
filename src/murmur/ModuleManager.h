// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_MODULEMANAGER_H_
#define MUMBLE_MURMUR_MODULEMANAGER_H_

#include "IServerModule.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>

class Server;

/**
 * @brief The ModuleManager class manages server modules.
 * 
 * This class is responsible for registering, initializing, and
 * providing access to server modules. It handles module lifecycle
 * and facilitates communication between modules.
 */
class ModuleManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for ModuleManager.
     * @param server Pointer to the Server instance
     * @param parent The parent QObject
     */
    explicit ModuleManager(Server *server, QObject *parent = nullptr);
    
    /**
     * @brief Destructor for ModuleManager.
     */
    ~ModuleManager();
    
    /**
     * @brief Register a module with the manager.
     * 
     * This adds the module to the registry but does not initialize it.
     * 
     * @param module Pointer to the module to register
     * @return True if registration was successful, false otherwise
     */
    bool registerModule(IServerModule *module);
    
    /**
     * @brief Initialize all registered modules.
     * 
     * @return True if all modules were initialized successfully, false otherwise
     */
    bool initializeAllModules();
    
    /**
     * @brief Initialize a specific module by name.
     * 
     * @param moduleName The name of the module to initialize
     * @return True if the module was initialized successfully, false otherwise
     */
    bool initializeModule(const QString &moduleName);
    
    /**
     * @brief Get a registered module by name.
     * 
     * @param moduleName The name of the module to retrieve
     * @return Pointer to the module, or nullptr if not found
     */
    IServerModule* getModule(const QString &moduleName) const;
    
    /**
     * @brief Get a registered module by name with type casting.
     * 
     * This template method allows for type-safe access to modules.
     * 
     * @tparam T The module type to cast to
     * @param moduleName The name of the module to retrieve
     * @return Pointer to the module cast to type T, or nullptr if not found
     */
    template<typename T>
    T* getModuleAs(const QString &moduleName) const {
        IServerModule *module = getModule(moduleName);
        return (module) ? qobject_cast<T*>(module) : nullptr;
    }
    
    /**
     * @brief Check if a module is registered.
     * 
     * @param moduleName The name of the module to check
     * @return True if the module is registered, false otherwise
     */
    bool hasModule(const QString &moduleName) const;
    
    /**
     * @brief Get a list of all registered module names.
     * 
     * @return List of module names
     */
    QStringList getModuleNames() const;
    
    /**
     * @brief Shut down all modules.
     * 
     * This should be called during server shutdown to clean up resources.
     */
    void shutdownAllModules();
    
    /**
     * @brief Send an event to all modules.
     * 
     * This allows for broadcasting events to all registered modules.
     * 
     * @param eventName The name of the event
     * @param data The event data
     */
    void broadcastEvent(const QString &eventName, const QVariant &data);

public slots:
    /**
     * @brief Handle module events.
     * 
     * This slot is connected to the moduleEvent signal of all registered modules.
     * 
     * @param eventName The name of the event
     * @param data The event data
     */
    void onModuleEvent(const QString &eventName, const QVariant &data);

signals:
    /**
     * @brief Signal emitted when a module is registered.
     * 
     * @param moduleName The name of the registered module
     */
    void moduleRegistered(const QString &moduleName);
    
    /**
     * @brief Signal emitted when a module is initialized.
     * 
     * @param moduleName The name of the initialized module
     * @param success Whether initialization was successful
     */
    void moduleInitialized(const QString &moduleName, bool success);
    
    /**
     * @brief Signal emitted when a module event is received.
     * 
     * This signal is emitted when a module event is received and can be
     * used by the server or other components to respond to module events.
     * 
     * @param moduleName The name of the module that emitted the event
     * @param eventName The name of the event
     * @param data The event data
     */
    void moduleEventReceived(const QString &moduleName, const QString &eventName, const QVariant &data);

private:
    Server *m_server; // Pointer to the server instance
    QHash<QString, IServerModule*> m_modules; // Registry of modules
};

#endif // MUMBLE_MURMUR_MODULEMANAGER_H_