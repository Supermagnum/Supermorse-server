// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PropagationModule.h"
#include "../Server.h"

#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include <QtCore/QRegularExpression>

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
    
    // Set up update timer
    connect(&m_updateTimer, &QTimer::timeout, this, &PropagationModule::updatePropagation);
}

PropagationModule::~PropagationModule() {
    // Stop the timer
    m_updateTimer.stop();
}

bool PropagationModule::initialize(Server *server) {
    if (!server) {
        qWarning() << "PropagationModule: Cannot initialize with null server";
        return false;
    }
    
    m_server = server;
    
    // Load configuration
    QSettings qs("mumble-server.ini", QSettings::IniFormat);
    qs.beginGroup("hf_propagation");
    
    // Check if HF band simulation is enabled
    bool enabled = qs.value("enabled", true).toBool();
    if (!enabled) {
        qWarning() << "PropagationModule: HF band simulation is disabled in configuration";
        qs.endGroup();
        return true; // Still considered initialized successfully
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
    if (autoSeason) {
        m_hfBandSimulation.setAutoTimeEnabled(true);
    } else {
        int season = qs.value("season", 0).toInt();
        m_hfBandSimulation.setSeason(season);
        m_hfBandSimulation.setAutoTimeEnabled(false);
    }
    
    // Set update interval (default: 30 minutes)
    int updateInterval = qs.value("update_interval", 30).toInt();
    m_updateTimer.start(updateInterval * 60 * 1000); // Convert minutes to milliseconds
    
    qs.endGroup();
    
    // Initial propagation update
    updatePropagation();
    
    qDebug() << "PropagationModule: Initialized";
    
    return true;
}

QString PropagationModule::name() const {
    return "PropagationModule";
}

QString PropagationModule::version() const {
    return "1.0.0";
}

QString PropagationModule::description() const {
    return "Manages HF band propagation simulation";
}

QVariant PropagationModule::getSetting(const QString &key, const QVariant &defaultValue) const {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would read from a settings store
    // For this simplified version, we just return the default value
    return defaultValue;
}

bool PropagationModule::setSetting(const QString &key, const QVariant &value) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would write to a settings store
    // For this simplified version, we just log the change
    qDebug() << "PropagationModule: Setting" << key << "to" << value;
    
    return true;
}

void PropagationModule::shutdown() {
    // Stop the timer
    m_updateTimer.stop();
    
    qDebug() << "PropagationModule: Shutdown";
}

float PropagationModule::calculatePropagation(ServerUser *user1, ServerUser *user2) {
    QMutexLocker locker(&m_mutex);
    
    // Get the grid locators
    QString grid1 = user1->qsMetadata.value("maidenheadgrid", "").toString();
    QString grid2 = user2->qsMetadata.value("maidenheadgrid", "").toString();
    
    if (grid1.isEmpty() || grid2.isEmpty()) {
        return 0.0f; // No propagation without grid locators
    }
    
    // Calculate signal strength between the grids
    return calculateSignalStrength(grid1, grid2);
}

bool PropagationModule::canCommunicate(ServerUser *user1, ServerUser *user2) {
    QMutexLocker locker(&m_mutex);
    
    // Calculate propagation
    float propagation = calculatePropagation(user1, user2);
    
    // Check if propagation is above the minimum threshold for communication
    return propagation >= 0.05f;
}

float PropagationModule::getSignalQuality(ServerUser *user1, ServerUser *user2) {
    QMutexLocker locker(&m_mutex);
    
    // Get propagation
    float propagation = calculatePropagation(user1, user2);
    
    // Apply any additional factors that affect signal quality
    // For example, user's radio equipment quality, power level, etc.
    
    // For this simplified version, we just return the propagation value
    return propagation;
}

void PropagationModule::getFadingEffects(float signalStrength, float &packetLoss, float &jitter, float &noiseFactor) {
    QMutexLocker locker(&m_mutex);
    
    // Calculate packet loss based on signal strength
    // Weak signals have higher packet loss
    packetLoss = 1.0f - signalStrength;
    packetLoss = qBound(0.0f, packetLoss, 1.0f);
    
    // Calculate jitter based on signal strength
    // Weak signals have higher jitter
    jitter = 1.0f - signalStrength;
    jitter = qBound(0.0f, jitter, 1.0f);
    
    // Calculate noise factor based on signal strength
    // Weak signals have higher noise
    noiseFactor = 1.0f - signalStrength;
    noiseFactor = qBound(0.0f, noiseFactor, 1.0f);
}

float PropagationModule::calculateSignalStrength(const QString &grid1, const QString &grid2) {
    QMutexLocker locker(&m_mutex);
    
    // Delegate to HFBandSimulation
    return m_hfBandSimulation.calculateSignalStrength(grid1, grid2);
}

int PropagationModule::recommendBand(float distance) {
    QMutexLocker locker(&m_mutex);
    
    // Delegate to HFBandSimulation
    return m_hfBandSimulation.recommendBand(distance);
}

int PropagationModule::getBandChannel(int band) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would look up the channel ID for a band
    // For this simplified version, we just return the band value as the channel ID
    return band;
}

int PropagationModule::getChannelBand(int channelId) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would look up the band for a channel
    // For this simplified version, we just return the channel ID as the band
    // if it's a valid band value
    
    // Check if channel ID is a valid band value
    if (channelId == 10 || channelId == 12 || channelId == 15 ||
        channelId == 17 || channelId == 20 || channelId == 30 ||
        channelId == 40 || channelId == 80 || channelId == 160) {
        return channelId;
    }
    
    // Not a band channel
    return 0;
}

void PropagationModule::setSolarFluxIndex(int sfi) {
    QMutexLocker locker(&m_mutex);
    
    // Delegate to HFBandSimulation
    m_hfBandSimulation.setSolarFluxIndex(sfi);
}

int PropagationModule::solarFluxIndex() const {
    // Delegate to HFBandSimulation
    return m_hfBandSimulation.solarFluxIndex();
}

void PropagationModule::setKIndex(int kIndex) {
    QMutexLocker locker(&m_mutex);
    
    // Delegate to HFBandSimulation
    m_hfBandSimulation.setKIndex(kIndex);
}

int PropagationModule::kIndex() const {
    // Delegate to HFBandSimulation
    return m_hfBandSimulation.kIndex();
}

void PropagationModule::updateAudioRouting(ServerUser *u1, ServerUser *u2) {
    QMutexLocker locker(&m_mutex);
    
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
        qDebug() << "PropagationModule: Audio routing between" << u1->qsName << "and" << u2->qsName
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
            qDebug() << "PropagationModule: Signal too weak, blocking audio between" 
                    << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would prevent audio packets from being transmitted
            return;
        }
        
        // 2. Apply packet loss simulation (signal dropouts)
        bool applyPacketLoss = (QRandomGenerator::global()->generateDouble() < packetLoss);
        if (applyPacketLoss) {
            // Randomly drop this audio update to simulate fading
            qDebug() << "PropagationModule: Simulating packet loss between" 
                    << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would randomly drop audio packets
            // For now, we just simulate the behavior
        }
        
        // 3. Apply noise to the audio signal
        if (noiseFactor > 0.1f) {
            qDebug() << "PropagationModule: Adding" << noiseFactor * 100.0f 
                    << "% noise to audio between"
                    << u1->qsName << "and" << u2->qsName;
            
            // In a real implementation, this would add white noise to audio samples
            // The intensity would be proportional to the noise factor
        }
        
        // 4. Apply jitter effects (timing variations)
        if (jitter > 0.2f) {
            qDebug() << "PropagationModule: Adding jitter factor" << jitter 
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
    QMutexLocker locker(&m_mutex);
    
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
    qDebug() << "PropagationModule: Open bands based on propagation:" << openBandsStr;
    
    // In a real implementation, this would update the channel links
    // based on the open bands
}

void PropagationModule::sendBandRecommendations(ServerUser *u, const QString &grid) {
    QMutexLocker locker(&m_mutex);
    
    // Get current time
    QDateTime now = QDateTime::currentDateTime();
    bool isDaytime = m_hfBandSimulation.calculateSolarZenithAngle(grid, now) < 90.0f;
    
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

void PropagationModule::updatePropagation() {
    QMutexLocker locker(&m_mutex);
    
    // Delegate to HFBandSimulation
    m_hfBandSimulation.updatePropagation();
}

void PropagationModule::onPropagationUpdated() {
    QMutexLocker locker(&m_mutex);
    
    // Get current propagation conditions
    int sfi = m_hfBandSimulation.solarFluxIndex();
    int kIndex = m_hfBandSimulation.kIndex();
    int season = m_hfBandSimulation.season();
    
    // Emit signal with updated conditions
    emit propagationConditionsChanged(sfi, kIndex, season);
    
    // Update channel links based on propagation
    updateChannelLinks();
    
    // Log the update
    QString seasonName;
    switch (season) {
        case 0: seasonName = "Winter"; break;
        case 1: seasonName = "Spring"; break;
        case 2: seasonName = "Summer"; break;
        case 3: seasonName = "Fall"; break;
        default: seasonName = "Unknown"; break;
    }
    
    qDebug() << "PropagationModule: Propagation updated: SFI=" << sfi
             << ", K-Index=" << kIndex << ", Season=" << seasonName;
}

void PropagationModule::onSignalStrengthChanged(const QString &grid1, const QString &grid2, float strength) {
    QMutexLocker locker(&m_mutex);
    
    // Log the update
    qDebug() << "PropagationModule: Signal strength changed between"
             << grid1 << "and" << grid2 << ":" << strength;
}

void PropagationModule::onMUFChanged(float muf) {
    QMutexLocker locker(&m_mutex);
    
    // Log the update
    qDebug() << "PropagationModule: Maximum Usable Frequency changed:" << muf << "MHz";
}

void PropagationModule::onExternalDataUpdated(const QString &source, bool success) {
    QMutexLocker locker(&m_mutex);
    
    // Log the update
    qDebug() << "PropagationModule: External data updated from" << source
             << ":" << (success ? "success" : "failure");
}

void PropagationModule::sendMessage(ServerUser *u, const QString &message) {
    QMutexLocker locker(&m_mutex);
    
    // In a real implementation, this would send a message to the user
    // For this simplified version, we just log the message
    qDebug() << "PropagationModule: Message to" << u->qsName << ":" << message;
    
    // Delegate to the server to send the message
    if (m_server) {
        // In a real implementation, this would call m_server->sendMessage(u, message)
        // For this simplified version, we just log that we would send the message
        qDebug() << "PropagationModule: Would send message to" << u->qsName << "via server";
    }
}