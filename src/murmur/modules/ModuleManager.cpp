// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ModuleManager.h"
#include "../Server.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>

ModuleManager::ModuleManager(Server *server, QObject *parent)
    : QObject(parent)
    , m_server(server) {
    // Create the thread pool with optimal number of threads
    m_threadPool = new ThreadPool(ThreadPool::optimalThreadCount(), this);
    qDebug() << "ModuleManager: Created thread pool with" << m_threadPool->threadCount() << "threads";
}

ModuleManager::~ModuleManager() {
    // Shutdown all modules before destruction
    shutdownAllModules();
    
    // Clear the module registry
    m_modules.clear();
    
    // The thread pool will be automatically deleted since it's a child QObject
}
void ModuleManager::broadcastEventParallel(const QString &eventName, const QVariant &data) {
    // Create a list of futures to track task completion
    std::vector<std::future<void>> futures;
    
    // Reserve space for the futures
    futures.reserve(m_modules.size());
    
    // Enqueue tasks for each module
    foreach (IServerModule *module, m_modules.values()) {
        // Create a copy of the event data and module pointer for the task
        auto task = [module, eventName, data]() {
            // Emit the event signal for this module
            emit module->moduleEvent(eventName, data);
        };
        
        // Enqueue the task and store the future
        futures.push_back(m_threadPool->enqueue(task));
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures) {
        future.wait();
    }
    
    qDebug() << "ModuleManager: Parallel broadcast of event" << eventName << "to" << m_modules.size() << "modules completed";
}

void ModuleManager::executeOnAllModules(const std::function<void(IServerModule*)>& func) {
    // Create a list of futures to track task completion
    std::vector<std::future<void>> futures;
    
    // Reserve space for the futures
    futures.reserve(m_modules.size());
    
    // Enqueue tasks for each module
    foreach (IServerModule *module, m_modules.values()) {
        // Create a task that will execute the function on the module
        auto task = [module, func]() {
            func(module);
        };
        
        // Enqueue the task and store the future
        futures.push_back(m_threadPool->enqueue(task));
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures) {
        future.wait();
    }
    
    qDebug() << "ModuleManager: Parallel execution on" << m_modules.size() << "modules completed";
}

bool ModuleManager::executeOnModule(const QString &moduleName, const std::function<void(IServerModule*)>& func) {
    // Check if the module exists
    if (!m_modules.contains(moduleName)) {
        qWarning() << "ModuleManager: Cannot execute on unknown module" << moduleName;
        return false;
    }
    
    // Get the module
    IServerModule *module = m_modules.value(moduleName);
    
    // Enqueue the task
    auto future = m_threadPool->enqueue([module, func]() {
        func(module);
    });
    
    // Wait for the task to complete
    future.wait();
    
    return true;
}
bool ModuleManager::registerModule(IServerModule *module) {
    if (!module) {
        qWarning() << "ModuleManager: Cannot register null module";
        return false;
    }
    
    // Get the module name
    QString moduleName = module->name();
    
    // Check if a module with this name is already registered
    if (m_modules.contains(moduleName)) {
        qWarning() << "ModuleManager: Module with name" << moduleName << "is already registered";
        return false;
    }
    
    // Add the module to the registry
    m_modules.insert(moduleName, module);
    
    // Connect the module's signals to our slots
    connect(module, &IServerModule::moduleEvent, this, &ModuleManager::onModuleEvent);
    
    // Emit the moduleRegistered signal
    emit moduleRegistered(moduleName);
    
    qDebug() << "ModuleManager: Registered module" << moduleName << "(" << module->version() << ")";
    
    return true;
}

bool ModuleManager::initializeAllModules() {
    bool allSuccess = true;
    
    // Initialize all registered modules
    foreach (const QString &moduleName, m_modules.keys()) {
        bool success = initializeModule(moduleName);
        allSuccess = allSuccess && success;
    }
    
    return allSuccess;
}

bool ModuleManager::initializeModule(const QString &moduleName) {
    // Check if the module exists
    if (!m_modules.contains(moduleName)) {
        qWarning() << "ModuleManager: Cannot initialize unknown module" << moduleName;
        return false;
    }
    
    // Get the module
    IServerModule *module = m_modules.value(moduleName);
    
    // Initialize the module
    bool success = module->initialize(m_server);
    
    // Emit the moduleInitialized signal
    emit moduleInitialized(moduleName, success);
    
    if (success) {
        qDebug() << "ModuleManager: Initialized module" << moduleName;
    } else {
        qWarning() << "ModuleManager: Failed to initialize module" << moduleName;
    }
    
    return success;
}

IServerModule* ModuleManager::getModule(const QString &moduleName) const {
    // Return the module or nullptr if not found
    return m_modules.value(moduleName, nullptr);
}

bool ModuleManager::hasModule(const QString &moduleName) const {
    return m_modules.contains(moduleName);
}

QStringList ModuleManager::getModuleNames() const {
    return m_modules.keys();
}

void ModuleManager::shutdownAllModules() {
    // Shutdown all modules in reverse order of registration
    QStringList moduleNames = m_modules.keys();
    for (int i = moduleNames.size() - 1; i >= 0; --i) {
        QString moduleName = moduleNames.at(i);
        IServerModule *module = m_modules.value(moduleName);
        
        if (module) {
            qDebug() << "ModuleManager: Shutting down module" << moduleName;
            module->shutdown();
        }
    }
}

void ModuleManager::broadcastEvent(const QString &eventName, const QVariant &data) {
    // Broadcast the event to all modules
    foreach (IServerModule *module, m_modules.values()) {
        // The modules will receive this event through the moduleEvent signal
        // which is connected to their respective slots
        emit module->moduleEvent(eventName, data);
    }
}

void ModuleManager::onModuleEvent(const QString &eventName, const QVariant &data) {
    // Get the sender module
    QObject *sender = QObject::sender();
    IServerModule *module = qobject_cast<IServerModule*>(sender);
    
    if (module) {
        QString moduleName = module->name();
        
        // Emit the moduleEventReceived signal
        emit moduleEventReceived(moduleName, eventName, data);
        
        // Optionally, handle specific events here
        // For now, we just log the event
        qDebug() << "ModuleManager: Received event" << eventName << "from module" << moduleName;
    }
}