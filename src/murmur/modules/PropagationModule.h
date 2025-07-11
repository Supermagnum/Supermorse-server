// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_PROPAGATIONMODULE_H_
#define MUMBLE_MURMUR_PROPAGATIONMODULE_H_

#include "IServerModule.h"
#include "HFBandSimulation.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QTimer>
#include <QtCore/QMutex>

class Server;
class ServerUser;
class Channel;

/**
 * @brief The PropagationModule class manages HF band propagation simulation.
 * 
 * This module encapsulates the HF band simulation functionality that was
 * previously integrated into the Server class. It handles propagation
 * calculations, signal quality, and band recommendations.
 */
class PropagationModule : public IServerModule {
    Q_OBJECT
public:
    /**
     * @brief Constructor for PropagationModule.
     * @param parent The parent QObject
     */
    explicit PropagationModule(QObject *parent = nullptr);
    
    /**
     * @brief Destructor for PropagationModule.
     */
    ~PropagationModule();
    
    // IServerModule interface implementation
    bool initialize(Server *server) override;
    QString name() const override;
    QString version() const override;
    QString description() const override;
    QVariant getSetting(const QString &key, const QVariant &defaultValue = QVariant()) const override;
    bool setSetting(const QString &key, const QVariant &value) override;
    void shutdown() override;
    
    /**
     * @brief Calculate the propagation between two users.
     * 
     * @param user1 The first user
     * @param user2 The second user
     * @return The signal strength between the users (0.0 to 1.0)
     */
    float calculatePropagation(ServerUser *user1, ServerUser *user2);
    
    /**
     * @brief Determine if two users can communicate.
     * 
     * @param user1 The first user
     * @param user2 The second user
     * @return True if communication is possible, false otherwise
     */
    bool canCommunicate(ServerUser *user1, ServerUser *user2);
    
    /**
     * @brief Get the signal quality between two users.
     * 
     * @param user1 The first user
     * @param user2 The second user
     * @return The signal quality (0.0 to 1.0)
     */
    float getSignalQuality(ServerUser *user1, ServerUser *user2);
    
    /**
     * @brief Get fading effects for a given signal strength.
     * 
     * @param signalStrength The signal strength
     * @param packetLoss Output parameter for packet loss
     * @param jitter Output parameter for jitter
     * @param noiseFactor Output parameter for noise factor
     */
    void getFadingEffects(float signalStrength, float &packetLoss, float &jitter, float &noiseFactor);
    
    /**
     * @brief Calculate the signal strength between two grid locators.
     * 
     * @param grid1 The first grid locator
     * @param grid2 The second grid locator
     * @return The signal strength (0.0 to 1.0)
     */
    float calculateSignalStrength(const QString &grid1, const QString &grid2);
    
    /**
     * @brief Recommend a band for a given distance.
     * 
     * @param distance The distance in kilometers
     * @return The recommended band in meters
     */
    int recommendBand(float distance);
    
    /**
     * @brief Get the channel ID for a specific band.
     * 
     * @param band The band in meters
     * @return The channel ID
     */
    int getBandChannel(int band);
    
    /**
     * @brief Get the band for a specific channel.
     * 
     * @param channelId The channel ID
     * @return The band in meters, or 0 if not a band channel
     */
    int getChannelBand(int channelId);
    
    /**
     * @brief Set the solar flux index.
     * 
     * @param sfi The solar flux index
     */
    void setSolarFluxIndex(int sfi);
    
    /**
     * @brief Get the solar flux index.
     * 
     * @return The solar flux index
     */
    int solarFluxIndex() const;
    
    /**
     * @brief Set the K-index.
     * 
     * @param kIndex The K-index
     */
    void setKIndex(int kIndex);
    
    /**
     * @brief Get the K-index.
     * 
     * @return The K-index
     */
    int kIndex() const;
    
    /**
     * @brief Update the audio routing between two users.
     * 
     * @param u1 The first user
     * @param u2 The second user
     */
    void updateAudioRouting(ServerUser *u1, ServerUser *u2);
    
    /**
     * @brief Update channel links based on propagation conditions.
     */
    void updateChannelLinks();
    
    /**
     * @brief Send band recommendations to a user.
     * 
     * @param u The user
     * @param grid The user's grid locator
     */
    void sendBandRecommendations(ServerUser *u, const QString &grid);
    
    /**
     * @brief Get the HFBandSimulation instance.
     * 
     * @return Pointer to the HFBandSimulation instance
     */
    HFBandSimulation* getHFBandSimulation() {
        return &m_hfBandSimulation;
    }

public slots:
    /**
     * @brief Update propagation conditions for all users.
     * 
     * This method is called periodically to update the propagation
     * conditions between all users.
     */
    void updatePropagation();
    
    /**
     * @brief Handle propagation updated event from HFBandSimulation.
     */
    void onPropagationUpdated();
    
    /**
     * @brief Handle signal strength changed event from HFBandSimulation.
     * 
     * @param grid1 The first grid locator
     * @param grid2 The second grid locator
     * @param strength The new signal strength
     */
    void onSignalStrengthChanged(const QString &grid1, const QString &grid2, float strength);
    
    /**
     * @brief Handle MUF changed event from HFBandSimulation.
     * 
     * @param muf The new Maximum Usable Frequency
     */
    void onMUFChanged(float muf);
    
    /**
     * @brief Handle external data updated event from HFBandSimulation.
     * 
     * @param source The data source
     * @param success Whether the update was successful
     */
    void onExternalDataUpdated(const QString &source, bool success);

signals:
    /**
     * @brief Signal emitted when the signal quality between two users changes.
     * 
     * @param userSession1 The first user's session ID
     * @param userSession2 The second user's session ID
     * @param quality The new signal quality
     */
    void signalQualityChanged(unsigned int userSession1, unsigned int userSession2, float quality);
    
    /**
     * @brief Signal emitted when propagation conditions change.
     * 
     * @param sfi The solar flux index
     * @param kIndex The K-index
     * @param season The season
     */
    void propagationConditionsChanged(int sfi, int kIndex, int season);

private:
    Server *m_server; // Pointer to the server instance
    HFBandSimulation m_hfBandSimulation; // HF band simulation engine
    QMutex m_mutex; // Mutex for thread safety
    QTimer m_updateTimer; // Timer for periodic updates
    
    /**
     * @brief Send a message to a user.
     * 
     * @param u The user to send the message to
     * @param message The message to send
     */
    void sendMessage(ServerUser *u, const QString &message);
};

#endif // MUMBLE_MURMUR_PROPAGATIONMODULE_H_