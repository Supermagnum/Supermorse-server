// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PropagationModule.h"
#include "Server.h"
#include "ServerUser.h"
#include "Channel.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include <QtCore/QRandomGenerator>

PropagationModule::PropagationModule(QObject *parent)
    : IServerModule(parent)
    , m_server(nullptr) {
    // Connect signals from HFBandSimulation
    connect(&m_hfBandSimulation, &HFBandSimulation::propagationUpdated, 
            this, &PropagationModule::onPropagationUpdated);
    connect(&m_hfBandSimulation, &HFBandSimulation::signalStrengthChanged, 
            this, &PropagationModule::onSignalStrengthChanged);
    connect(&m_hfBandSimulation, &HFBandSimulation::mufChanged, 
            this, &PropagationModule::onMUFChanged);
    connect(&m_hfBandSimulation, &HFBandSimulation::externalDataUpdated, 
            this, &PropagationModule::onExternalDataUpdated);
    
    // Connect timer for propagation updates
    connect(&m_updateTimer, &QTimer::timeout, 
            this, &PropagationModule::updatePropagation);
}

PropagationModule::~PropagationModule() {
    m_updateTimer.stop();
}

bool PropagationModule::initialize(Server *server) {
    if (!server) {
        qWarning() << "PropagationModule: Cannot initialize with null server";
        return false;
    }
    
    m_server = server;
    
    // Initialize the HF band simulation
    m_hfBandSimulation.initialize();
    
    // Load propagation parameters from configuration
    QSettings qs("mumble-server.ini", QSettings::IniFormat);
    qs.beginGroup("hf_propagation");
    
    // Check if HF band simulation is enabled
    bool enabled = qs.value("enabled", true).toBool();
    if (!enabled) {
        qWarning() << "PropagationModule: HF band simulation is disabled in configuration";
        qs.endGroup();
        return true; // Module is initialized but disabled
    }
    
    // Set external data source settings
    bool useExternalData = qs.value("use_external_data", false).toBool();
    m_hfBandSimulation.setUseExternalData(useExternalData);
    
    if (useExternalData) {
        // Set DXView.org data settings
        bool useDXViewData = qs.value("use_dxview_data", false).toBool();
        m_hfBandSimulation.setUseDXViewData(useDXViewData);
        
        // Set SWPC data settings
        bool useSWPCData = qs.value("use_swpc_data", false).toBool();
        m_hfBandSimulation.setUseSWPCData(useSWPCData);
        
        qWarning() << "PropagationModule: Using external data sources:"
                  << "DXView.org:" << (useDXViewData ? "enabled" : "disabled")
                  << "SWPC:" << (useSWPCData ? "enabled" : "disabled");
    }
    
    // Set solar flux index (default: 120)
    int sfi = qs.value("solar_flux_index", 120).toInt();
    m_hfBandSimulation.setSolarFluxIndex(sfi);
    
    // Set K-index (default: 3)
    int kIndex = qs.value("k_index", 3).toInt();
    m_hfBandSimulation.setKIndex(kIndex);
    
    // Set season (0=Winter, 1=Spring, 2=Summer, 3=Fall, default: auto)
    bool autoSeason = qs.value("auto_season", true).toBool();
    m_hfBandSimulation.setAutoTimeEnabled(autoSeason);
    
    if (!autoSeason) {
        int season = qs.value("season", 0).toInt();
        m_hfBandSimulation.setSeason(season);
    }
    
    // Set update interval (default: 5 minutes)
    int updateInterval = qs.value("update_interval", 5).toInt();
    m_updateTimer.start(updateInterval * 60 * 1000); // Convert minutes to milliseconds
    
    qs.endGroup();
    
    // Initial propagation update
    updatePropagation();
    
    qDebug() << "PropagationModule: Initialized successfully";
    return true;
}

QString PropagationModule::name() const {
    return "PropagationModule";
}

QString PropagationModule::version() const {
    return "1.0.0";
}

QString PropagationModule::description() const {
    return "Provides HF band propagation simulation for amateur radio";
}

QVariant PropagationModule::getSetting(const QString &key, const QVariant &defaultValue) const {
    QMutexLocker locker(&m_mutex);
    
    if (key == "solarFluxIndex") {
        return m_hfBandSimulation.solarFluxIndex();
    } else if (key == "kIndex") {
        return m_hfBandSimulation.kIndex();
    } else if (key == "autoTimeEnabled") {
        return m_hfBandSimulation.isAutoTimeEnabled();
    } else if (key == "useExternalData") {
        return m_hfBandSimulation.useExternalData();
    } else if (key == "useDXViewData") {
        return m_hfBandSimulation.useDXViewData();
    } else if (key == "useSWPCData") {
        return m_hfBandSimulation.useSWPCData();
    }
    
    return defaultValue;
}

bool PropagationModule::setSetting(const QString &key, const QVariant &value) {
    QMutexLocker locker(&m_mutex);
    
    if (key == "solarFluxIndex") {
        m_hfBandSimulation.setSolarFluxIndex(value.toInt());
        return true;
    } else if (key == "kIndex") {
        m_hfBandSimulation.setKIndex(value.toInt());
        return true;
    } else if (key == "autoTimeEnabled") {
        m_hfBandSimulation.setAutoTimeEnabled(value.toBool());
        return true;
    } else if (key == "useExternalData") {
        m_hfBandSimulation.setUseExternalData(value.toBool());
        return true;
    } else if (key == "useDXViewData") {
        m_hfBandSimulation.setUseDXViewData(value.toBool());
        return true;
    } else if (key == "useSWPCData") {
        m_hfBandSimulation.setUseSWPCData(value.toBool());
        return true;
    }
    
    return false;
}

void PropagationModule::shutdown() {
    m_updateTimer.stop();
    
    // Signal that the module is shutting down
    QVariant data;
    emit moduleEvent("shutdown", data);
    
    qDebug() << "PropagationModule: Shut down";
}

float PropagationModule::calculatePropagation(ServerUser *user1, ServerUser *user2) {
    return m_hfBandSimulation.calculatePropagation(user1, user2);
}

bool PropagationModule::canCommunicate(ServerUser *user1, ServerUser *user2) {
    return m_hfBandSimulation.canCommunicate(user1, user2);
}

float PropagationModule::getSignalQuality(ServerUser *user1, ServerUser *user2) {
    return m_hfBandSimulation.getSignalQuality(user1, user2);
}

void PropagationModule::getFadingEffects(float signalStrength, float &packetLoss, float &jitter, float &noiseFactor) {
    m_hfBandSimulation.getFadingEffects(signalStrength, packetLoss, jitter, noiseFactor);
}

float PropagationModule::calculateSignalStrength(const QString &grid1, const QString &grid2) {
    return m_hfBandSimulation.calculateSignalStrength(grid1, grid2);
}

int PropagationModule::recommendBand(float distance) {
    return m_hfBandSimulation.recommendBand(distance);
}

int PropagationModule::getBandChannel(int band) {
    return m_hfBandSimulation.getBandChannel(band);
}

int PropagationModule::getChannelBand(int channelId) {
    return m_hfBandSimulation.getChannelBand(channelId);
}

void PropagationModule::setSolarFluxIndex(int sfi) {
    m_hfBandSimulation.setSolarFluxIndex(sfi);
}

int PropagationModule::solarFluxIndex() const {
    return m_hfBandSimulation.solarFluxIndex();
}

void PropagationModule::setKIndex(int kIndex) {
    m_hfBandSimulation.setKIndex(kIndex);
}

int PropagationModule::kIndex() const {
    return m_hfBandSimulation.kIndex();
}

void PropagationModule::updatePropagation() {
    QMutexLocker locker(&m_mutex);
    
    // Update the HF band simulation
    m_hfBandSimulation.updatePropagation();
    
    // Emit an event that propagation has been updated
    QVariantMap data;
    data["solarFluxIndex"] = m_hfBandSimulation.solarFluxIndex();
    data["kIndex"] = m_hfBandSimulation.kIndex();
    
    emit moduleEvent("propagationUpdated", QVariant(data));
}

void PropagationModule::onPropagationUpdated() {
    if (!m_server) {
        return;
    }
    
    // Get current propagation conditions
    int sfi = m_hfBandSimulation.solarFluxIndex();
    int kIndex = m_hfBandSimulation.kIndex();
    int season = m_hfBandSimulation.season();
    
    // Emit signal with updated propagation conditions
    emit propagationConditionsChanged(sfi, kIndex, season);
    
    // Create a detailed message about propagation conditions
    QString seasonName;
    switch (season) {
        case 0: seasonName = "Winter"; break;
        case 1: seasonName = "Spring"; break;
        case 2: seasonName = "Summer"; break;
        case 3: seasonName = "Fall"; break;
        default: seasonName = "Unknown"; break;
    }
    
    QString message = QString("Propagation conditions updated: Solar Flux Index: %1, K-Index: %2, Season: %3")
                        .arg(sfi).arg(kIndex).arg(seasonName);
    
    // Log the update
    qWarning() << "PropagationModule: " << message;
    
    // Notify users of the updated propagation conditions
    QHashIterator<unsigned int, ServerUser *> i(m_server->qhUsers);
    while (i.hasNext()) {
        i.next();
        ServerUser *u = i.value();
        
        if (u && u->sState == ServerUser::Authenticated) {
            sendMessage(u, message);
            
            // If user has a grid locator, send band recommendations
            QString grid = u->qsMetadata.value("maidenheadgrid", "").toString();
            if (!grid.isEmpty()) {
                sendBandRecommendations(u, grid);
            }
        }
    }
    
    // Update channel links based on propagation
    updateChannelLinks();
}

void PropagationModule::onSignalStrengthChanged(const QString &grid1, const QString &grid2, float strength) {
    if (!m_server) {
        return;
    }
    
    qWarning() << "PropagationModule: Signal strength changed between" << grid1 << "and" << grid2 << ":" << strength;
    
    // Find users with these grid locators and update their audio routing
    QHashIterator<unsigned int, ServerUser *> i(m_server->qhUsers);
    while (i.hasNext()) {
        i.next();
        ServerUser *u1 = i.value();
        
        if (u1 && u1->sState == ServerUser::Authenticated) {
            QString u1Grid = u1->qsMetadata.value("maidenheadgrid", "").toString();
            if (u1Grid == grid1) {
                QHashIterator<unsigned int, ServerUser *> j(m_server->qhUsers);
                while (j.hasNext()) {
                    j.next();
                    ServerUser *u2 = j.value();
                    
                    if (u2 && u2->sState == ServerUser::Authenticated && u1 != u2) {
                        QString u2Grid = u2->qsMetadata.value("maidenheadgrid", "").toString();
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

void PropagationModule::onMUFChanged(float muf) {
    if (!m_server) {
        return;
    }
    
    qWarning() << "PropagationModule: Maximum Usable Frequency changed:" << muf << "MHz";
    
    // Notify users of the MUF change
    QString message = QString("Maximum Usable Frequency changed: %1 MHz").arg(muf);
    
    QHashIterator<unsigned int, ServerUser *> i(m_server->qhUsers);
    while (i.hasNext()) {
        i.next();
        ServerUser *u = i.value();
        
        if (u && u->sState == ServerUser::Authenticated) {
            sendMessage(u, message);
        }
    }
}

void PropagationModule::onExternalDataUpdated(const QString &source, bool success) {
    if (!m_server) {
        return;
    }
    
    QString message = QString("External data from %1 %2")
                        .arg(source)
                        .arg(success ? "updated successfully" : "update failed");
    
    qWarning() << "PropagationModule: " << message;
    
    // Notify users of the external data update
    QHashIterator<unsigned int, ServerUser *> i(m_server->qhUsers);
    while (i.hasNext()) {
        i.next();
        ServerUser *u = i.value();
        
        if (u && u->sState == ServerUser::Authenticated) {
            sendMessage(u, message);
        }
    }
}

void PropagationModule::updateAudioRouting(ServerUser *u1, ServerUser *u2) {
    if (!m_server || !u1 || !u2) {
        return;
    }
    
    // Get the signal quality between the users (graduated scale, not binary)
    float signalQuality = getSignalQuality(u1, u2);
    
    // Get the users' grid locators
    QString grid1 = u1->qsMetadata.value("maidenheadgrid", "").toString();
    QString grid2 = u2->qsMetadata.value("maidenheadgrid", "").toString();
    
    if (!grid1.isEmpty() && !grid2.isEmpty()) {
        // Calculate fading effects based on signal quality
        float packetLoss = 0.0f;
        float jitter = 0.0f;
        float noiseFactor = 0.0f;
        
        getFadingEffects(signalQuality, packetLoss, jitter, noiseFactor);
        
        // Log the audio routing update with detailed signal metrics
        qWarning() << "PropagationModule: Audio routing between" << u1->qsName << "and" << u2->qsName
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
            qWarning() << "PropagationModule: Signal too weak, blocking audio between" 
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would prevent audio packets from being transmitted
            return;
        }
        
        // 2. Apply packet loss simulation (signal dropouts)
        bool applyPacketLoss = (QRandomGenerator::global()->generateDouble() < packetLoss);
        if (applyPacketLoss) {
            // Randomly drop this audio update to simulate fading
            qWarning() << "PropagationModule: Simulating packet loss between" 
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would randomly drop audio packets
            // For now, we just simulate the behavior
        }
        
        // 3. Apply noise to the audio signal
        if (noiseFactor > 0.1f) {
            qWarning() << "PropagationModule: Adding" << noiseFactor * 100.0f 
                      << "% noise to audio between"
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would add white noise to audio samples
            // The intensity would be proportional to the noise factor
        }
        
        // 4. Apply jitter effects (timing variations)
        if (jitter > 0.2f) {
            qWarning() << "PropagationModule: Adding jitter factor" << jitter 
                      << " to audio between"
                      << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would vary packet timing
            // and potentially reorder packets to simulate propagation variations
        }
        
        // Emit a signal to notify clients about the signal quality
        emit signalQualityChanged(u1->uiSession, u2->uiSession, signalQuality);
    }
}

void PropagationModule::updateChannelLinks() {
    if (!m_server) {
        return;
    }
    
    // Get current propagation conditions
    int sfi = m_hfBandSimulation.solarFluxIndex();
    int kIndex = m_hfBandSimulation.kIndex();
    
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
    qWarning() << "PropagationModule: Open bands based on propagation:" << openBandsStr;
    
    // In a real implementation, this would update the channel links
    // based on the open bands
}

void PropagationModule::sendBandRecommendations(ServerUser *u, const QString &grid) {
    if (!m_server || !u) {
        return;
    }
    
    // Get current time
    QDateTime now = QDateTime::currentDateTime();
    
    // Determine if it's daytime at the user's location
    // We're making a simplification here for the example
    int hour = now.time().hour();
    bool isDaytime = (hour >= 6 && hour <= 18);
    
    // Create a message with band recommendations
    QString message = QString("Band recommendations for %1 (%2):\n").arg(grid).arg(isDaytime ? "Day" : "Night");
    
    // Get solar conditions
    int sfi = m_hfBandSimulation.solarFluxIndex();
    int kIndex = m_hfBandSimulation.kIndex();
    
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

void PropagationModule::sendMessage(ServerUser *u, const QString &message) {
    if (!m_server || !u) {
        return;
    }
    
    // In a real implementation, this would use the Server's message sending capabilities
    // For now, we just log the message
    qWarning() << "PropagationModule: Sending message to" << u->qsName << ":" << message;
}