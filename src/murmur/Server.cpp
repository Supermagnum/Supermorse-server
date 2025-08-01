// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Server.h"
#include "modules/UserDataModule.h"
#include "modules/PropagationModule.h"
#include "modules/UserStatisticsModule.h"
#include "database/MariaDBConnectionParameter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtCore/QRegularExpression>
#include <QtCore/QRandomGenerator>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QHostAddress>

// This is a simplified version of the Server.cpp file
// In a real implementation, this would include all the functionality
// from the original Server.cpp file, with modifications for HF band simulation

Server::Server(unsigned int snum, const ::mumble::db::ConnectionParameter &connectionParam, QObject *parent) : QThread(parent), iServerNum(snum), m_dbWrapper(connectionParam) {
    // Create the module manager
    m_moduleManager = new ModuleManager(this, this);
    
    // Initialize HFBandSimulation pointer to nullptr - will be set up by PropagationModule later
    m_pHFBandSimulation = nullptr;
    
    // Initialize the server
    qsRegName = "Supermorse Mumble Server";
}

Server::~Server() {
    // No need to delete m_pHFBandSimulation as it's owned by PropagationModule
    delete m_moduleManager; // Clean up the module manager
}

void Server::initialize() {
    // Initialize the server
    
    // Load configuration
    QSettings qs("mumble-server.ini", QSettings::IniFormat);
    
    // Set up channels from configuration
    setupChannels(qs);
    
    // Register modules
    registerModules();
    
    // Initialize all modules
    m_moduleManager->initializeAllModules();
    
    // Initialize the HF band simulation for backward compatibility
    initializeHFBandSimulation();
}

void Server::registerModules() {
    // Create and register the user data module
    UserDataModule *userDataModule = new UserDataModule(this);
    m_moduleManager->registerModule(userDataModule);
    
    // Create and register the propagation module
    PropagationModule *propagationModule = new PropagationModule(this);
    m_moduleManager->registerModule(propagationModule);
    
    // Create and register the user statistics module
    UserStatisticsModule *userStatsModule = new UserStatisticsModule(this);
    m_moduleManager->registerModule(userStatsModule);
    
    qDebug() << "Server: Registered modules:" << m_moduleManager->getModuleNames().join(", ");
}

void Server::setupChannels(QSettings &qs) {
    // Set up channels from configuration
    qs.beginGroup("channels");
    QStringList channels = qs.childKeys();
    
    for (const QString &key : channels) {
        int id = key.toInt();
        QString name = qs.value(key).toString();
        
        // Create the channel
        Channel *c = new Channel(id, name);
        qhChannels.insert(id, c);
    }
    
    // Set up channel links
    qs.endGroup();
    qs.beginGroup("channel_links");
    QStringList links = qs.childKeys();
    
    for (const QString &key : links) {
        int id = key.toInt();
        QStringList linkedChannels = qs.value(key).toString().split(",");
        
        Channel *c = qhChannels.value(id);
        if (c) {
            for (const QString &linkedId : linkedChannels) {
                Channel *linked = qhChannels.value(linkedId.toInt());
                if (linked) {
                    c->qsPermLinks.insert(linked->iId);
                }
            }
        }
    }
    
    // Set up channel descriptions
    qs.endGroup();
    qs.beginGroup("channel_description");
    QStringList descriptions = qs.childKeys();
    
    for (const QString &key : descriptions) {
        int id = key.toInt();
        QString description = qs.value(key).toString();
        
        Channel *c = qhChannels.value(id);
        if (c) {
            c->qsDesc = description;
        }
    }
    
    qs.endGroup();
}

void Server::initializeHFBandSimulation() {
    // Get the PropagationModule from the ModuleManager
    PropagationModule* propagationModule = static_cast<PropagationModule*>(
        m_moduleManager->getModule("PropagationModule"));
    
    if (!propagationModule) {
        qWarning() << "Failed to get PropagationModule from ModuleManager. HF band simulation not initialized.";
        return;
    }
    
    // Get the HFBandSimulation instance from the PropagationModule
    m_pHFBandSimulation = propagationModule->getHFBandSimulation();
    
    if (!m_pHFBandSimulation) {
        qWarning() << "Failed to get HFBandSimulation from PropagationModule. HF band simulation not initialized.";
        return;
    }
    
    // Load propagation parameters from configuration
    QSettings qs("mumble-server.ini", QSettings::IniFormat);
    qs.beginGroup("hf_propagation");
    
    // Check if HF band simulation is enabled
    bool enabled = qs.value("enabled", true).toBool();
    if (!enabled) {
        qWarning() << "HF band simulation is disabled in configuration";
        qs.endGroup();
        return;
    }
    
    // Set external data source settings
    bool useExternalData = qs.value("use_external_data", false).toBool();
    m_pHFBandSimulation->setUseExternalData(useExternalData);
    
    if (useExternalData) {
        // Set DXView.org data settings
        bool useDXViewData = qs.value("use_dxview_data", false).toBool();
        m_pHFBandSimulation->setUseDXViewData(useDXViewData);
        
        // Set SWPC data settings
        bool useSWPCData = qs.value("use_swpc_data", false).toBool();
        m_pHFBandSimulation->setUseSWPCData(useSWPCData);
        
        qWarning() << "HF band simulation using external data sources:"
                  << "DXView.org:" << (useDXViewData ? "enabled" : "disabled")
                  << "SWPC:" << (useSWPCData ? "enabled" : "disabled");
    }
    
    // Set solar flux index (default: 120)
    int sfi = qs.value("solar_flux_index", 120).toInt();
    m_pHFBandSimulation->setSolarFluxIndex(sfi);
    
    // Set K-index (default: 3)
    int kIndex = qs.value("k_index", 3).toInt();
    m_pHFBandSimulation->setKIndex(kIndex);
    
    // Set season (0=Winter, 1=Spring, 2=Summer, 3=Fall, default: auto)
    bool autoSeason = qs.value("auto_season", true).toBool();
    if (autoSeason) {
        m_pHFBandSimulation->setAutoTimeEnabled(true);
    } else {
        int season = qs.value("season", 0).toInt();
        m_pHFBandSimulation->setSeason(season);
        m_pHFBandSimulation->setAutoTimeEnabled(false);
    }
    
    // Set update interval (default: 30 minutes)
    int updateInterval = qs.value("update_interval", 30).toInt();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Server::updateHFBandPropagation);
    timer->start(updateInterval * 60 * 1000); // Convert minutes to milliseconds
    
    qs.endGroup();
    
    // Connect signals and slots for propagation updates
    connect(m_pHFBandSimulation, &HFBandSimulation::propagationUpdated, this, &Server::onPropagationUpdated);
    connect(m_pHFBandSimulation, &HFBandSimulation::signalStrengthChanged, this, &Server::onSignalStrengthChanged);
    connect(m_pHFBandSimulation, &HFBandSimulation::mufChanged, this, &Server::onMUFChanged);
    connect(m_pHFBandSimulation, &HFBandSimulation::externalDataUpdated, this, &Server::onExternalDataUpdated);
    
    // Initial propagation update
    updateHFBandPropagation();
}

void Server::onPropagationUpdated() {
    // This method is called when propagation conditions change
    // Update the server state based on the new propagation conditions
    
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in onPropagationUpdated";
        return;
    }
    
    // Get current propagation conditions
    int sfi = m_pHFBandSimulation->solarFluxIndex();
    int kIndex = m_pHFBandSimulation->kIndex();
    int season = m_pHFBandSimulation->season();
    QString seasonName;
    
    switch (season) {
        case 0: seasonName = "Winter"; break;
        case 1: seasonName = "Spring"; break;
        case 2: seasonName = "Summer"; break;
        case 3: seasonName = "Fall"; break;
        default: seasonName = "Unknown"; break;
    }
    
    // Create a detailed message about propagation conditions
    QString message = QString("Propagation conditions updated: Solar Flux Index: %1, K-Index: %2, Season: %3")
                        .arg(sfi).arg(kIndex).arg(seasonName);
    
    // Log the update
    qWarning() << "HF Propagation updated:" << message;
    
    // Notify users of the updated propagation conditions
    foreach(ServerUser *u, qhUsers) {
        if (u->iId > 0) {
            sendMessage(u, message);
            
            // If user has a grid locator, send band recommendations
            QString grid = u->qmUserData.value("maidenheadgrid", "");
            if (!grid.isEmpty()) {
                sendBandRecommendations(u, grid);
            }
        }
    }
    
    // Update channel links based on propagation
    updateChannelLinks();
}

void Server::onSignalStrengthChanged(const QString &grid1, const QString &grid2, float strength) {
    // This method is called when the signal strength between two grid locators changes
    qWarning() << "Signal strength changed between" << grid1 << "and" << grid2 << ":" << strength;
    
    // Find users with these grid locators and update their audio routing
    foreach(ServerUser *u1, qhUsers) {
        if (u1->iId > 0) {
            QString u1Grid = u1->qmUserData.value("maidenheadgrid", "");
            if (u1Grid == grid1) {
                foreach(ServerUser *u2, qhUsers) {
                    if (u2->iId > 0 && u1 != u2) {
                        QString u2Grid = u2->qmUserData.value("maidenheadgrid", "");
                        if (u2Grid == grid2) {
                            // Update audio routing between these users
                            updateAudioRouting(u1, u2);
                        }
                    }
                }
            }
        }
    }
}

void Server::onMUFChanged(float muf) {
    // This method is called when the Maximum Usable Frequency changes
    qWarning() << "Maximum Usable Frequency changed:" << muf << "MHz";
    
    // Notify users of the MUF change
    QString message = QString("Maximum Usable Frequency changed: %1 MHz").arg(muf);
    
    foreach(ServerUser *u, qhUsers) {
        if (u->iId > 0) {
            sendMessage(u, message);
        }
    }
}

void Server::sendMessage(ServerUser *u, const QString &message) {
    // Send a text message to a user
    // Create and send text message
    QString msg = message;
    sendTextMessage(nullptr, u, false, msg);
    
    // In a real implementation, this would use the actual sendMessage method
    // For this simplified version, we'll just log the message
    qWarning() << "Sending message to user" << u->qsName << ":" << message;
}

void Server::sendTextMessage(Channel *cChannel, ServerUser *pUser, bool tree, const QString &text) {
    // Implementation for sending text messages
    qWarning() << "Sending text message to" << (pUser ? pUser->qsName : "all users in channel") 
              << ":" << text
              << (tree ? "(including subchannels)" : "");
    
    // In a real implementation, this would create and send a TextMessage
    // For this simplified version, we just log the message
}

void Server::onExternalDataUpdated(const QString &source, bool success) {
    // Handle external data updates for propagation modeling
    qWarning() << "External data updated from" << source << (success ? "successfully" : "with errors");
    
    if (success) {
        // Update propagation model with new data
        updateHFBandPropagation();
    } else {
        qWarning() << "Failed to update external data from" << source;
    }
}

void Server::regSslError(const QList<QSslError> &errors) {
    // Handle SSL errors during registration
    qWarning() << "SSL errors during registration:";
    for (const QSslError &error : errors) {
        qWarning() << " -" << error.errorString();
    }
}

void Server::finished() {
    // Called when the server thread finishes
    qWarning() << "Server thread finished";
    bRunning = false;
}

void Server::update() {
    // Update server state periodically
    qWarning() << "Server update";
    // In a full implementation, this would update various server stats
}

void Server::newClient() {
    // Handle new client connection
    qWarning() << "New client connection";
    // In a full implementation, this would accept the new connection and create a ServerUser
}

void Server::connectionClosed(QAbstractSocket::SocketError error, const QString &errorString) {
    // Handle closed connection
    qWarning() << "Connection closed with error:" << errorString << "(" << error << ")";
}

void Server::sslError(const QList<QSslError> &errors) {
    // Handle SSL errors
    qWarning() << "SSL errors:";
    for (const QSslError &error : errors) {
        qWarning() << " -" << error.errorString();
    }
}

void Server::message(Mumble::Protocol::TCPMessageType type, const QByteArray &data, ServerUser *cCon) {
    // Process incoming message
    qWarning() << "Received message of type" << static_cast<int>(type) 
              << "from" << (cCon ? cCon->qsName : "unknown");
}

void Server::checkTimeout() {
    // Check for user timeouts
    qWarning() << "Checking for user timeouts";
    // In a full implementation, this would disconnect users who haven't sent data in a while
}

void Server::tcpTransmitData(QByteArray data, unsigned int id) {
    // Transmit data over TCP
    qWarning() << "Transmitting" << data.size() << "bytes of TCP data to user ID" << id;
}

void Server::doSync(unsigned int id) {
    // Synchronize server state with client
    qWarning() << "Synchronizing server state with user ID" << id;
}

void Server::encrypted() {
    // Handle encrypted connection
    qWarning() << "Encrypted connection established";
}

void Server::udpActivated(int socketDescriptor) {
    // Handle UDP socket activation
    qWarning() << "UDP socket activated on descriptor" << socketDescriptor;
}

void Server::customEvent(QEvent *evt) {
    // Handle custom event
    if (evt->type() == EXEC_QEVENT) {
        ExecEvent *e = static_cast<ExecEvent *>(evt);
        e->execute();
    }
}

void Server::run() {
    // Main server thread loop
    qWarning() << "Server thread starting";
    bRunning = true;
    
    while (bRunning) {
        // In a real implementation, this would process incoming connections and messages
        // For this simplified version, we'll just sleep
        QThread::msleep(100);
    }
    
    qWarning() << "Server thread exiting";
}

void SslServer::incomingConnection(qintptr socketDescriptor) {
    // Handle incoming SSL connection
    QSslSocket *qss = new QSslSocket(this);
    
    if (qss->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "New SSL connection from" << qss->peerAddress().toString();
        qlSockets.append(qss);
    } else {
        delete qss;
    }
}

// Implementation of ExecEvent methods
ExecEvent::ExecEvent(boost::function<void()> f) 
    : QEvent(static_cast<QEvent::Type>(EXEC_QEVENT)), func(f) {
}

void ExecEvent::execute() {
    func();
}

bool Server::canCommunicate(ServerUser *u1, ServerUser *u2) {
    // Check if two users can communicate based on HF band simulation
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in canCommunicate";
        return false;
    }
    return m_pHFBandSimulation->canCommunicate(u1, u2);
}

float Server::calculatePropagation(ServerUser *u1, ServerUser *u2) {
    // Calculate the propagation between two users
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in calculatePropagation";
        return 0.0f;
    }
    return m_pHFBandSimulation->calculatePropagation(u1, u2);
}

float Server::calculateSignalStrength(const QString &grid1, const QString &grid2) {
    // Calculate the signal strength between two grid locators
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in calculateSignalStrength";
        return 0.0f;
    }
    return m_pHFBandSimulation->calculateSignalStrength(grid1, grid2);
}

int Server::recommendBand(float distance) {
    // Recommend a band for a given distance
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in recommendBand";
        return 20; // Default to 20m band if not initialized
    }
    return m_pHFBandSimulation->recommendBand(distance);
}

void Server::userStateChanged(ServerUser *u) {
    // This method is called when a user's state changes
    // Update the user's state and notify other users as needed
    
    // Check if the user has a grid locator in their metadata
    QString grid = u->qmUserData.value("maidenheadgrid", "");
    if (!grid.isEmpty()) {
        // Validate the grid locator format (should be 4 or 6 characters)
        QRegularExpression gridRegex("^[A-R]{2}[0-9]{2}([a-x]{2})?$");
        if (!gridRegex.match(grid).hasMatch()) {
            // Invalid grid locator format
            sendMessage(u, QString("Warning: Invalid Maidenhead grid locator format: %1. Please use format like 'AB12' or 'AB12cd'.").arg(grid));
            return;
        }
        
        // User has a valid grid locator, update propagation
        qWarning() << "User" << u->qsName << "has grid locator:" << grid;
        
        // Send band recommendations to the user
        sendBandRecommendations(u, grid);
        
        // Update audio routing for this user with all other users
        foreach(ServerUser *other, qhUsers) {
            if (other->iId > 0 && other != u) {
                updateAudioRouting(u, other);
            }
        }
        
        // Update propagation for all users
        updateHFBandPropagation();
    } else {
        // User doesn't have a grid locator, send a reminder
        sendMessage(u, "Please set your Maidenhead grid locator in your profile for HF band simulation.");
    }
    
    // Check if the user has a preferred HF band in their metadata
    QString preferredBand = u->qmUserData.value("preferredhfband", "");
    if (!preferredBand.isEmpty()) {
        qWarning() << "User" << u->qsName << "has preferred HF band:" << preferredBand;
        
        // Move the user to their preferred band channel if it exists
        bool ok;
        int bandId = preferredBand.toInt(&ok);
        if (ok && qhChannels.contains(bandId)) {
            Channel *c = qhChannels.value(bandId);
            if (c) {
                // In a real implementation, this would move the user to the channel
                qWarning() << "Moving user" << u->qsName << "to preferred band channel:" << c->qsName;
            }
        }
    }
}

void Server::updateHFBandPropagation() {
    // Update the HF band propagation for all users
    // This is called when a user's state changes or
    // when propagation conditions change
    
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in updateHFBandPropagation";
        return;
    }
    
    // Update the HF band simulation
    m_pHFBandSimulation->updatePropagation();
    
    // Update audio routing for all users
    foreach(ServerUser *u1, qhUsers) {
        if (u1->iId > 0) {
            foreach(ServerUser *u2, qhUsers) {
                if (u2->iId > 0 && u1 != u2) {
                    updateAudioRouting(u1, u2);
                }
            }
        }
    }
}

void Server::updateAudioRouting(ServerUser *u1, ServerUser *u2) {
    // Update the audio routing between two users based on propagation
    
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in updateAudioRouting";
        return;
    }
    
    // Get the signal quality between the users (graduated scale, not binary)
    float signalQuality = m_pHFBandSimulation->getSignalQuality(u1, u2);
    
    // Get the users' grid locators
    QString grid1 = u1->qmUserData.value("maidenheadgrid", "");
    QString grid2 = u2->qmUserData.value("maidenheadgrid", "");
    
    if (!grid1.isEmpty() && !grid2.isEmpty()) {
        // Calculate fading effects based on signal quality
        float packetLoss = 0.0f;
        float jitter = 0.0f;
        float noiseFactor = 0.0f;
        
        m_pHFBandSimulation->getFadingEffects(signalQuality, packetLoss, jitter, noiseFactor);
        
        // Log the audio routing update with detailed signal metrics
        qWarning() << "Audio routing between" << u1->qsName << "and" << u2->qsName
                  << ": Signal quality:" << signalQuality 
                  << ", Packet loss:" << packetLoss * 100.0f << "%"
                  << ", Jitter:" << jitter
                  << ", Noise:" << noiseFactor;
        
        // Apply graduated audio degradation effects
        // In a real implementation, this would modify audio packets in the transmission pipeline
        
        // 1. Determine if we should block audio completely (very poor signal)
        bool blockAudio = (signalQuality < 0.05f);
        
        if (blockAudio) {
            // Signal too weak for any communication
            qWarning() << "Signal too weak, blocking audio between" 
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would prevent audio packets from being transmitted
            return;
        }
        
        // 2. Apply packet loss simulation (signal dropouts)
        bool applyPacketLoss = (QRandomGenerator::global()->generateDouble() < packetLoss);
        if (applyPacketLoss) {
            // Randomly drop this audio update to simulate fading
            qWarning() << "Simulating packet loss between" 
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would randomly drop audio packets
            // For now, we just simulate the behavior
        }
        
        // 3. Apply noise to the audio signal
        if (noiseFactor > 0.1f) {
            qWarning() << "Adding" << noiseFactor * 100.0f 
                      << "% noise to audio between"
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would add white noise to audio samples
            // The intensity would be proportional to the noise factor
        }
        
        // 4. Apply jitter effects (timing variations)
        if (jitter > 0.2f) {
            qWarning() << "Adding jitter factor" << jitter 
                      << " to audio between"
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would vary packet timing
            // and potentially reorder packets to simulate propagation variations
        }
        
        // Emit a signal to notify clients about the signal quality
        emit signalQualityChanged(u1->uiSession, u2->uiSession, signalQuality);
    }
}

void Server::updateChannelLinks() {
    // Update channel links based on current propagation conditions
    
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in updateChannelLinks";
        return;
    }
    
    // Get current propagation conditions
    int sfi = m_pHFBandSimulation->solarFluxIndex();
    int kIndex = m_pHFBandSimulation->kIndex();
    
    // Determine which bands are open based on conditions
    QList<int> openBands;
    
    // High solar activity opens higher bands
    if (sfi > 150) {
        // Good conditions for 10m, 12m, 15m
        openBands << 10 << 12 << 15;
    } else if (sfi > 100) {
        // Moderate conditions for 15m, 17m, 20m
        openBands << 15 << 17 << 20;
    } else {
        // Poor conditions, lower bands only
        openBands << 40 << 80 << 160;
    }
    
    // High K-index disrupts higher bands
    if (kIndex > 5) {
        // Remove higher bands during geomagnetic disturbances
        openBands.removeAll(10);
        openBands.removeAll(12);
        openBands.removeAll(15);
    }
    
    // Log the open bands
    QString openBandsStr;
    for (int band : openBands) {
        openBandsStr += QString::number(band) + "m, ";
    }
    if (!openBandsStr.isEmpty()) {
        openBandsStr.chop(2); // Remove trailing comma and space
    }
    qWarning() << "Open bands based on propagation:" << openBandsStr;
    
    // In a real implementation, this would update the channel links
    // based on the open bands
}

void Server::sendBandRecommendations(ServerUser *u, const QString &grid) {
    // Send band recommendations to a user based on their grid locator
    
    if (!m_pHFBandSimulation) {
        qWarning() << "HFBandSimulation not initialized in sendBandRecommendations";
        return;
    }
    
    // Get current time
    QDateTime now = QDateTime::currentDateTime();
    bool isDaytime = m_pHFBandSimulation->calculateSolarZenithAngle(grid, now) < 90.0f;
    
    // Create a message with band recommendations
    QString message = QString("Band recommendations for %1 (%2):\n").arg(grid).arg(isDaytime ? "Day" : "Night");
    
    // Get solar conditions
    int sfi = m_pHFBandSimulation->solarFluxIndex();
    int kIndex = m_pHFBandSimulation->kIndex();
    
    // Add solar conditions to the message
    message += QString("Solar Flux Index: %1, K-Index: %2\n").arg(sfi).arg(kIndex);
    
    // Recommend bands based on time of day and solar conditions
    if (isDaytime) {
        if (sfi > 150) {
            message += "Excellent conditions for DX on higher bands.\n";
            message += "Recommended bands: 10m, 12m, 15m, 17m, 20m";
        } else if (sfi > 100) {
            message += "Good conditions for DX on mid-range bands.\n";
            message += "Recommended bands: 15m, 17m, 20m, 30m";
        } else {
            message += "Fair conditions, focus on lower bands.\n";
            message += "Recommended bands: 20m, 30m, 40m";
        }
    } else {
        message += "Nighttime conditions favor lower bands.\n";
        message += "Recommended bands: 40m, 80m, 160m";
        
        // During solar maximum, 20m can stay open at night
        if (sfi > 150) {
            message += ", 20m";
        }
    }
    
    // Send the message to the user
    sendMessage(u, message);
}