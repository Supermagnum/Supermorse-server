// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_HFBANDSIMULATION_H_
#define MUMBLE_MURMUR_HFBANDSIMULATION_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QPair>

class ServerUser;

/**
 * @brief The HFBandSimulation class simulates HF band propagation.
 * 
 * This class provides simulation of HF radio propagation conditions
 * based on various factors such as time of day, season, solar activity,
 * and geomagnetic conditions.
 */
class HFBandSimulation : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for HFBandSimulation.
     * @param parent The parent QObject
     */
    explicit HFBandSimulation(QObject *parent = nullptr);
    
    /**
     * @brief Destructor for HFBandSimulation.
     */
    ~HFBandSimulation();
    
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
     * @brief Calculate the signal strength between two grid locators.
     * 
     * @param grid1 The first grid locator
     * @param grid2 The second grid locator
     * @return The signal strength (0.0 to 1.0)
     */
    float calculateSignalStrength(const QString &grid1, const QString &grid2);
    
    /**
     * @brief Calculate the distance between two grid locators.
     * 
     * @param grid1 The first grid locator
     * @param grid2 The second grid locator
     * @return The distance in kilometers
     */
    float calculateDistance(const QString &grid1, const QString &grid2);
    
    /**
     * @brief Get the GPS coordinates from a grid locator.
     * 
     * @param grid The grid locator
     * @param latitude Output parameter for latitude
     * @param longitude Output parameter for longitude
     */
    void gridToCoordinates(const QString &grid, float &latitude, float &longitude);
    
    /**
     * @brief Get a grid locator from GPS coordinates.
     * 
     * @param latitude The latitude
     * @param longitude The longitude
     * @param precision The precision (4 or 6)
     * @return The grid locator
     */
    QString coordinatesToGrid(float latitude, float longitude, int precision = 6);
    
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
    int solarFluxIndex() const { return m_solarFluxIndex; }
    
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
    int kIndex() const { return m_kIndex; }
    
    /**
     * @brief Set the season.
     * 
     * @param season The season (0=Winter, 1=Spring, 2=Summer, 3=Fall)
     */
    void setSeason(int season);
    
    /**
     * @brief Get the season.
     * 
     * @return The season
     */
    int season() const { return m_season; }
    
    /**
     * @brief Set whether automatic time-based updates are enabled.
     * 
     * When enabled, the season is automatically determined based on the current date.
     * 
     * @param enabled Whether auto time is enabled
     */
    void setAutoTimeEnabled(bool enabled);
    
    /**
     * @brief Get whether automatic time-based updates are enabled.
     * 
     * @return True if auto time is enabled, false otherwise
     */
    bool autoTimeEnabled() const { return m_autoTimeEnabled; }
    
    /**
     * @brief Set whether external data sources are used.
     * 
     * @param enabled Whether external data is used
     */
    void setUseExternalData(bool enabled);
    
    /**
     * @brief Get whether external data sources are used.
     * 
     * @return True if external data is used, false otherwise
     */
    bool useExternalData() const { return m_useExternalData; }
    
    /**
     * @brief Set whether DXView.org data is used.
     * 
     * @param enabled Whether DXView data is used
     */
    void setUseDXViewData(bool enabled);
    
    /**
     * @brief Get whether DXView.org data is used.
     * 
     * @return True if DXView data is used, false otherwise
     */
    bool useDXViewData() const { return m_useDXViewData; }
    
    /**
     * @brief Set whether SWPC data is used.
     * 
     * @param enabled Whether SWPC data is used
     */
    void setUseSWPCData(bool enabled);
    
    /**
     * @brief Get whether SWPC data is used.
     * 
     * @return True if SWPC data is used, false otherwise
     */
    bool useSWPCData() const { return m_useSWPCData; }
    
    /**
     * @brief Calculate the solar zenith angle for a given location and time.
     * 
     * @param grid The grid locator
     * @param dateTime The date and time
     * @return The solar zenith angle in degrees
     */
    float calculateSolarZenithAngle(const QString &grid, const QDateTime &dateTime);
    
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
     * @brief Recommend a band for a given distance.
     * 
     * @param distance The distance in kilometers
     * @return The recommended band in meters
     */
    int recommendBand(float distance);
    
    /**
     * @brief Calculate the Maximum Usable Frequency (MUF).
     * 
     * @param distance The distance in kilometers
     * @return The MUF in MHz
     */
    float calculateMUF(float distance);
    
    /**
     * @brief Update propagation conditions.
     */
    void updatePropagation();

signals:
    /**
     * @brief Signal emitted when propagation conditions are updated.
     */
    void propagationUpdated();
    
    /**
     * @brief Signal emitted when the signal strength between two grid locators changes.
     * 
     * @param grid1 The first grid locator
     * @param grid2 The second grid locator
     * @param strength The new signal strength
     */
    void signalStrengthChanged(const QString &grid1, const QString &grid2, float strength);
    
    /**
     * @brief Signal emitted when the Maximum Usable Frequency changes.
     * 
     * @param muf The new MUF
     */
    void mufChanged(float muf);
    
    /**
     * @brief Signal emitted when external data is updated.
     * 
     * @param source The data source
     * @param success Whether the update was successful
     */
    void externalDataUpdated(const QString &source, bool success);

private:
    bool m_autoTimeEnabled; // Whether automatic time-based updates are enabled
    bool m_useExternalData; // Whether external data sources are used
    bool m_useDXViewData; // Whether DXView.org data is used
    bool m_useSWPCData; // Whether SWPC data is used
    
    int m_solarFluxIndex; // Solar flux index
    int m_kIndex; // K-index
    int m_season; // Season (0=Winter, 1=Spring, 2=Summer, 3=Fall)
    
    float m_muf; // Maximum Usable Frequency
    
    QHash<QPair<QString, QString>, float> m_signalStrengthCache; // Cache for signal strength calculations
    
    QTimer m_updateTimer; // Timer for periodic updates
    
    /**
     * @brief Update the season based on the current date.
     */
    void updateSeason();
    
    /**
     * @brief Update external data from sources.
     */
    void updateExternalData();
    
    /**
     * @brief Update data from DXView.org.
     * 
     * @return True if update was successful, false otherwise
     */
    bool updateDXViewData();
    
    /**
     * @brief Update data from SWPC.
     * 
     * @return True if update was successful, false otherwise
     */
    bool updateSWPCData();
    
    /**
     * @brief Calculate the critical frequency (foF2).
     * 
     * @return The critical frequency in MHz
     */
    float calculateCriticalFrequency();
    
    /**
     * @brief Calculate the F layer height.
     * 
     * @return The F layer height in kilometers
     */
    float calculateFLayerHeight();
};

#endif // MUMBLE_MURMUR_HFBANDSIMULATION_H_