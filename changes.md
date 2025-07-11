# Changes to HF Fading Algorithm

## Overview

Improved the packet loss calculation in the HF band simulation to make fading sound more natural and less robotic. The changes were implemented in the `getFadingEffects` method in `src/murmur/HFBandSimulation.cpp`.

## Previous Implementation

The original implementation used a simple power function with basic randomization:

```cpp
// Calculate packet loss - increases rapidly as signal degrades
// Signal fading in HF often manifests as complete dropouts
packetLoss = qPow(baseDegradation, 1.5f); // Non-linear curve

// Add randomization to packet loss to simulate fading
packetLoss *= (0.8f + (QRandomGenerator::global()->generateDouble() * 0.4f));
packetLoss = qBound(0.0f, packetLoss, 0.95f); // Max 95% packet loss
```

This approach produced predictable and somewhat mechanical-sounding fading patterns that didn't accurately simulate the complex behavior of real HF propagation.

## New Implementation

The new implementation uses a multi-component fading model that better simulates real HF propagation characteristics:

```cpp
// Calculate packet loss - using multi-component fading model to sound more natural
// HF propagation has both slow fading and fast flutter components

// Get current time for time-based fading components
qint64 currentTimeMs = QDateTime::currentMSecsSinceEpoch();

// Slow fading component (changes over seconds)
float slowFadePeriod = 5000.0f + (2000.0f * QRandomGenerator::global()->generateDouble()); // 5-7 seconds
float slowFadePhase = (currentTimeMs % static_cast<qint64>(slowFadePeriod)) / slowFadePeriod;
float slowFadeComponent = 0.5f * (1.0f + sin(2.0f * M_PI * slowFadePhase));

// Fast fading/flutter component (rapid variations)
float fastFadePeriod = 100.0f + (300.0f * QRandomGenerator::global()->generateDouble()); // 100-400ms
float fastFadePhase = (currentTimeMs % static_cast<qint64>(fastFadePeriod)) / fastFadePeriod;
float fastFadeComponent = 0.3f * (1.0f + sin(2.0f * M_PI * fastFadePhase * 3.0f));

// Random component for unpredictable variations
float randomComponent = 0.2f * QRandomGenerator::global()->generateDouble();

// Calculate base packet loss from signal degradation (still non-linear)
float basePacketLoss = qPow(baseDegradation, 1.3f);

// Occasional deep fades/dropouts (more likely with worse signals)
bool deepFade = QRandomGenerator::global()->generateDouble() < (0.05f + (0.15f * baseDegradation));
float deepFadeFactor = deepFade ? (0.7f + (0.3f * QRandomGenerator::global()->generateDouble())) : 0.0f;

// Combine all components
packetLoss = basePacketLoss * (0.5f + (0.3f * slowFadeComponent) + 
                              (0.1f * fastFadeComponent) + 
                              randomComponent + 
                              deepFadeFactor);

// Ensure packet loss is within valid range
packetLoss = qBound(0.0f, packetLoss, 0.95f); // Max 95% packet loss
```

## Key Improvements

1. **Multiple Temporal Components**: 
   - Slow fading (5-7 second period) to simulate gradual ionospheric changes
   - Fast fading/flutter (100-400ms period) to simulate rapid multipath effects

2. **Time-Based Calculations**:
   - Uses the current system time to create smooth, continuous transitions between fading states
   - Prevents abrupt, unnatural changes in audio quality

3. **Randomized Elements**:
   - Varying periods for both slow and fast fading components
   - Random component for unpredictable small variations

4. **Occasional Deep Fades**:
   - Probabilistic model for complete signal dropouts
   - More likely to occur when the base signal is already degraded
   - Creates authentic-sounding momentary losses that are characteristic of HF propagation

5. **Fine-Tuned Parameters**:
   - Adjusted the exponent in the power function (from 1.5 to 1.3) for a more gradual degradation curve
   - Weighted combination of components for realistic fading behavior

These changes make the fading sound more natural and realistic while maintaining appropriate overall signal degradation based on the base parameters. The sinusoidal components with varying periods, combined with the random elements and probabilistic deep fades, create a much more authentic representation of how HF signals actually behave in real-world conditions.

# Module Reorganization - 2025-07-11

## Overview

Organized all module-related files into a dedicated `modules` directory within the `src/murmur` directory. This improves project structure and maintainability by grouping related files together and making the module system more explicit in the codebase.

# Modular Server Architecture - 2025-07-10

## Overview

Refactored the server to use a modular architecture with separate modules for user data handling and propagation simulation. This makes the code more maintainable and extensible by clearly separating concerns and making it easy to add new modules in the future.

## Previous Implementation

The original implementation had all functionality directly integrated into the Server class:

```cpp
// Server.h
class Server : public QThread {
private:
    // Direct integration of HF band simulation
    HFBandSimulation m_hfBandSimulation;
    
    // User data handling mixed with other server functionality
    void processUserData();
    // ...other methods...
};
```

This tight coupling made it difficult to extend the server with new functionality and created maintenance challenges when updating specific components.

## New Implementation

The new implementation uses a module-based architecture with these components:

1. **Module Interface** - Defines the contract for all server modules:

```cpp
// IServerModule.h
class IServerModule {
public:
    virtual ~IServerModule() = default;
    virtual bool initialize() = 0;
    virtual void cleanup() = 0;
    virtual QString name() const = 0;
};
```

2. **Module Manager** - Handles registration and coordination of modules:

```cpp
// ModuleManager.h
class ModuleManager : public QObject {
private:
    QHash<QString, IServerModule*> m_modules;
    
public:
    bool registerModule(IServerModule* module);
    IServerModule* getModule(const QString& name);
    bool initializeModules();
    void cleanupModules();
};
```

3. **Specialized Modules** - User data and propagation simulation:

```cpp
// UserDataModule.h
class UserDataModule : public QObject, public IServerModule {
public:
    bool initialize() override;
    void cleanup() override;
    QString name() const override { return "UserData"; }
    
    // User data specific functionality
    // ...
};

// PropagationModule.h
class PropagationModule : public QObject, public IServerModule {
private:
    HFBandSimulation* m_simulation;
    
public:
    bool initialize() override;
    void cleanup() override;
    QString name() const override { return "Propagation"; }
    
    // Access to the HF band simulation
    HFBandSimulation* getSimulation() const { return m_simulation; }
};
```

4. **Updated Server Class** - Now uses modules instead of direct implementation:

```cpp
// Server.h
class Server : public QThread {
private:
    ModuleManager* m_moduleManager;
    HFBandSimulation* m_pHFBandSimulation; // Pointer for backward compatibility
    
    // ...
};

// Server.cpp
bool Server::initialize() {
    // Initialize module manager
    m_moduleManager = new ModuleManager(this);
    
    // Register modules
    m_moduleManager->registerModule(new UserDataModule(this));
    m_moduleManager->registerModule(new PropagationModule(this));
    
    // Initialize all modules
    if (!m_moduleManager->initializeModules()) {
        return false;
    }
    
    // Get propagation module for backward compatibility
    PropagationModule* propModule = 
        static_cast<PropagationModule*>(m_moduleManager->getModule("Propagation"));
    if (propModule) {
        m_pHFBandSimulation = propModule->getSimulation();
    }
    
    return true;
}
```

## Key Improvements

1. **Separation of Concerns**:
   - Each module has a clearly defined responsibility
   - User data handling and propagation simulation are now independent

2. **Improved Extensibility**:
   - New functionality can be added by creating new modules
   - Existing modules can be enhanced without affecting others

3. **Better Maintainability**:
   - Smaller, focused components are easier to understand and maintain
   - Changes to one module don't require changes to others

4. **Backward Compatibility**:
   - Server maintains the same interface for client code
   - Existing functionality continues to work as before

5. **Future-Proof Architecture**:
   - Framework for adding new radio features, security modules, etc.
   - Clear pathway for continuing development of the server