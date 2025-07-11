// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HFBandSimulation.h"
#include "../User.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QRandomGenerator>
#include <QtCore/QNetworkAccessManager>
#include <QtCore/QNetworkReply>
#include <QtCore/QNetworkRequest>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <cmath>

const float PI = 3.14159265358979323846f;

HFBandSimulation::HFBandSimulation(QObject *parent)
    : QObject(parent)
    , m_autoTimeEnabled(true)
    , m_useExternalData(false)
    , m_useDXViewData(false)
    , m_useSWPCData(false)
    , m_solarFluxIndex(120)
    , m_kIndex(3)
    , m_season(0)
    , m_muf(0.0f) {
    // Set up the update timer
    connect(&m_updateTimer, &QTimer::timeout, this, &HFBandSimulation::updatePropagation);
    
    // Initial update
    updateSeason();
}

HFBandSimulation::~HFBandSimulation() {
    // Stop the timer
    m_updateTimer.stop();
}

float HFBandSimulation::calculatePropagation(ServerUser *user1, ServerUser *user2) {
    // Get the grid locators
    QString grid1 = user1->qsMetadata.value("maidenheadgrid", "").toString();
    QString grid2 = user2->qsMetadata.value("maidenheadgrid", "").toString();
    
    if (grid1.isEmpty() || grid2.isEmpty()) {
        return 0.0f; // No propagation without grid locators
    }
    
    // Calculate signal strength between the grids
    return calculateSignalStrength(grid1, grid2);
}

bool HFBandSimulation::canCommunicate(ServerUser *user1, ServerUser *user2) {
    // Calculate propagation
    float propagation = calculatePropagation(user1, user2);
    
    // Check if propagation is above the minimum threshold for communication
    return propagation >= 0.05f;
}

float HFBandSimulation::getSignalQuality(ServerUser *user1, ServerUser *user2) {
    // Get propagation
    float propagation = calculatePropagation(user1, user2);
    
    // Apply any additional factors that affect signal quality
    // For example, user's radio equipment quality, power level, etc.
    
    // For this simplified version, we just return the propagation value
    return propagation;
}

float HFBandSimulation::calculateSignalStrength(const QString &grid1, const QString &grid2) {
    // Check if the signal strength is already cached
    QPair<QString, QString> gridPair(grid1, grid2);
    if (m_signalStrengthCache.contains(gridPair)) {
        return m_signalStrengthCache.value(gridPair);
    }
    
    // Calculate the distance between the grids
    float distance = calculateDistance(grid1, grid2);
    
    // Get the current time
    QDateTime now = QDateTime::currentDateTime();
    
    // Calculate the solar zenith angle at both locations
    float sza1 = calculateSolarZenithAngle(grid1, now);
    float sza2 = calculateSolarZenithAngle(grid2, now);
    
    // Determine if it's day or night at each location
    bool isDaytime1 = sza1 < 90.0f;
    bool isDaytime2 = sza2 < 90.0f;
    
    // Calculate the critical frequency (foF2)
    float foF2 = calculateCriticalFrequency();
    
    // Calculate the F layer height
    float fLayerHeight = calculateFLayerHeight();
    
    // Calculate the Maximum Usable Frequency (MUF)
    float muf = calculateMUF(distance);
    
    // Update the MUF member variable
    if (m_muf != muf) {
        m_muf = muf;
        emit mufChanged(muf);
    }
    
    // Determine the best band for the distance
    int bestBand = recommendBand(distance);
    
    // Calculate the signal strength based on various factors
    float strength = 0.0f;
    
    // 1. Distance factor (inversely proportional to distance)
    float distanceFactor = 1.0f / (1.0f + distance / 1000.0f);
    
    // 2. Time of day factor
    float timeOfDayFactor = 0.0f;
    if (isDaytime1 && isDaytime2) {
        // Both locations in daylight
        timeOfDayFactor = 1.0f;
    } else if (!isDaytime1 && !isDaytime2) {
        // Both locations in darkness
        timeOfDayFactor = 0.8f;
    } else {
        // One location in daylight, one in darkness
        timeOfDayFactor = 0.5f;
    }
    
    // 3. Solar activity factor
    float solarActivityFactor = m_solarFluxIndex / 200.0f;
    solarActivityFactor = qBound(0.1f, solarActivityFactor, 1.0f);
    
    // 4. Geomagnetic activity factor (K-index, inversely proportional)
    float geomagneticFactor = 1.0f - (m_kIndex / 9.0f);
    geomagneticFactor = qBound(0.1f, geomagneticFactor, 1.0f);
    
    // 5. Season factor
    float seasonFactor = 0.0f;
    switch (m_season) {
        case 0: // Winter
            seasonFactor = 0.7f;
            break;
        case 1: // Spring
            seasonFactor = 0.9f;
            break;
        case 2: // Summer
            seasonFactor = 1.0f;
            break;
        case 3: // Fall
            seasonFactor = 0.8f;
            break;
        default:
            seasonFactor = 0.8f;
            break;
    }
    
    // 6. Random factor (to simulate fading, sporadic-E, etc.)
    float randomFactor = 0.8f + 0.2f * QRandomGenerator::global()->generateDouble();
    
    // Combine all factors to calculate signal strength
    strength = distanceFactor * timeOfDayFactor * solarActivityFactor * geomagneticFactor * seasonFactor * randomFactor;
    
    // Ensure the strength is between 0 and 1
    strength = qBound(0.0f, strength, 1.0f);
    
    // Cache the signal strength
    m_signalStrengthCache.insert(gridPair, strength);
    
    // Also cache the reverse grid pair with the same strength
    m_signalStrengthCache.insert(QPair<QString, QString>(grid2, grid1), strength);
    
    // Emit the signal strength changed signal
    emit signalStrengthChanged(grid1, grid2, strength);
    
    return strength;
}

float HFBandSimulation::calculateDistance(const QString &grid1, const QString &grid2) {
    // Convert grid locators to coordinates
    float lat1, lon1, lat2, lon2;
    gridToCoordinates(grid1, lat1, lon1);
    gridToCoordinates(grid2, lat2, lon2);
    
    // Convert degrees to radians
    lat1 = lat1 * PI / 180.0f;
    lon1 = lon1 * PI / 180.0f;
    lat2 = lat2 * PI / 180.0f;
    lon2 = lon2 * PI / 180.0f;
    
    // Calculate distance using the Haversine formula
    float dlon = lon2 - lon1;
    float dlat = lat2 - lat1;
    float a = std::sin(dlat/2) * std::sin(dlat/2) +
              std::cos(lat1) * std::cos(lat2) *
              std::sin(dlon/2) * std::sin(dlon/2);
    float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    float distance = 6371.0f * c; // Earth's radius in km
    
    return distance;
}

void HFBandSimulation::gridToCoordinates(const QString &grid, float &latitude, float &longitude) {
    // Ensure the grid is at least 4 characters
    if (grid.length() < 4) {
        latitude = 0.0f;
        longitude = 0.0f;
        return;
    }
    
    // Field (first pair of letters)
    int longitudeField = grid[0].toUpper().toLatin1() - 'A';
    int latitudeField = grid[1].toUpper().toLatin1() - 'A';
    
    // Square (second pair of digits)
    int longitudeSquare = grid[2].toLatin1() - '0';
    int latitudeSquare = grid[3].toLatin1() - '0';
    
    // Calculate the longitude
    longitude = longitudeField * 20.0f + longitudeSquare * 2.0f - 180.0f;
    
    // Calculate the latitude
    latitude = latitudeField * 10.0f + latitudeSquare - 90.0f;
    
    // If 6-character locator, add subsquare precision
    if (grid.length() >= 6) {
        // Subsquare (third pair of letters)
        int longitudeSubsquare = grid[4].toLower().toLatin1() - 'a';
        int latitudeSubsquare = grid[5].toLower().toLatin1() - 'a';
        
        // Add subsquare precision
        longitude += longitudeSubsquare * 2.0f / 24.0f;
        latitude += latitudeSubsquare / 24.0f;
    }
    
    // Center of the grid square
    longitude += 1.0f;
    latitude += 0.5f;
}

QString HFBandSimulation::coordinatesToGrid(float latitude, float longitude, int precision) {
    // Ensure longitude is in the range -180 to 180
    while (longitude < -180.0f) longitude += 360.0f;
    while (longitude > 180.0f) longitude -= 360.0f;
    
    // Ensure latitude is in the range -90 to 90
    latitude = qBound(-90.0f, latitude, 90.0f);
    
    // Calculate field (first pair of letters)
    int longitudeField = static_cast<int>((longitude + 180.0f) / 20.0f);
    int latitudeField = static_cast<int>((latitude + 90.0f) / 10.0f);
    
    // Calculate square (second pair of digits)
    int longitudeSquare = static_cast<int>(((longitude + 180.0f) - longitudeField * 20.0f) / 2.0f);
    int latitudeSquare = static_cast<int>((latitude + 90.0f) - latitudeField * 10.0f);
    
    // Calculate subsquare (third pair of letters)
    int longitudeSubsquare = static_cast<int>(((longitude + 180.0f) - longitudeField * 20.0f - longitudeSquare * 2.0f) * 12.0f);
    int latitudeSubsquare = static_cast<int>(((latitude + 90.0f) - latitudeField * 10.0f - latitudeSquare) * 24.0f);
    
    // Build the grid locator
    QString grid;
    grid += QChar('A' + longitudeField);
    grid += QChar('A' + latitudeField);
    grid += QChar('0' + longitudeSquare);
    grid += QChar('0' + latitudeSquare);
    
    // Add subsquare for 6-character precision
    if (precision >= 6) {
        grid += QChar('a' + longitudeSubsquare);
        grid += QChar('a' + latitudeSubsquare);
    }
    
    return grid;
}

void HFBandSimulation::setSolarFluxIndex(int sfi) {
    if (m_solarFluxIndex != sfi) {
        m_solarFluxIndex = sfi;
        
        // Clear the signal strength cache
        m_signalStrengthCache.clear();
        
        // Emit the propagation updated signal
        emit propagationUpdated();
    }
}

void HFBandSimulation::setKIndex(int kIndex) {
    if (m_kIndex != kIndex) {
        m_kIndex = kIndex;
        
        // Clear the signal strength cache
        m_signalStrengthCache.clear();
        
        // Emit the propagation updated signal
        emit propagationUpdated();
    }
}

void HFBandSimulation::setSeason(int season) {
    if (m_season != season) {
        m_season = season;
        
        // Clear the signal strength cache
        m_signalStrengthCache.clear();
        
        // Emit the propagation updated signal
        emit propagationUpdated();
    }
}

void HFBandSimulation::setAutoTimeEnabled(bool enabled) {
    m_autoTimeEnabled = enabled;
    
    if (enabled) {
        // Update the season based on the current date
        updateSeason();
    }
}

void HFBandSimulation::setUseExternalData(bool enabled) {
    m_useExternalData = enabled;
    
    if (enabled) {
        // Update external data
        updateExternalData();
    }
}

void HFBandSimulation::setUseDXViewData(bool enabled) {
    m_useDXViewData = enabled;
    
    if (enabled && m_useExternalData) {
        // Update DXView data
        updateDXViewData();
    }
}

void HFBandSimulation::setUseSWPCData(bool enabled) {
    m_useSWPCData = enabled;
    
    if (enabled && m_useExternalData) {
        // Update SWPC data
        updateSWPCData();
    }
}

float HFBandSimulation::calculateSolarZenithAngle(const QString &grid, const QDateTime &dateTime) {
    // Convert grid locator to coordinates
    float latitude, longitude;
    gridToCoordinates(grid, latitude, longitude);
    
    // Calculate the day of the year (0-365)
    int dayOfYear = dateTime.date().dayOfYear() - 1;
    
    // Calculate the hour of the day (0-24)
    float hourOfDay = dateTime.time().hour() + dateTime.time().minute() / 60.0f;
    
    // Calculate the solar declination
    float declination = 23.45f * std::sin(2.0f * PI * (284.0f + dayOfYear) / 365.0f);
    
    // Calculate the time correction
    float timeCorrection = 4.0f * longitude - 60.0f * (dateTime.offsetFromUtc() / 3600.0f);
    
    // Calculate the solar hour angle
    float hourAngle = 15.0f * (hourOfDay + timeCorrection / 60.0f - 12.0f);
    
    // Convert degrees to radians
    float latitudeRad = latitude * PI / 180.0f;
    float declinationRad = declination * PI / 180.0f;
    float hourAngleRad = hourAngle * PI / 180.0f;
    
    // Calculate the solar zenith angle
    float cosZenith = std::sin(latitudeRad) * std::sin(declinationRad) +
                      std::cos(latitudeRad) * std::cos(declinationRad) * std::cos(hourAngleRad);
    
    // Ensure the cosine is in the range -1 to 1
    cosZenith = qBound(-1.0f, cosZenith, 1.0f);
    
    // Convert cosine to angle in degrees
    float zenith = std::acos(cosZenith) * 180.0f / PI;
    
    return zenith;
}

void HFBandSimulation::getFadingEffects(float signalStrength, float &packetLoss, float &jitter, float &noiseFactor) {
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

int HFBandSimulation::recommendBand(float distance) {
    // Calculate the MUF
    float muf = calculateMUF(distance);
    
    // Determine the best band based on the MUF
    // and various factors such as time of day, season, etc.
    
    // For distances less than 500 km
    if (distance < 500.0f) {
        // Local communication, use higher bands
        return 20; // 20m band
    }
    
    // For medium distances (500-2000 km)
    if (distance < 2000.0f) {
        // Check if MUF supports higher bands
        if (muf > 21.0f) {
            return 15; // 15m band
        } else if (muf > 14.0f) {
            return 20; // 20m band
        } else {
            return 40; // 40m band
        }
    }
    
    // For long distances (2000+ km)
    // Check if MUF supports higher bands
    if (muf > 28.0f) {
        return 10; // 10m band
    } else if (muf > 24.0f) {
        return 12; // 12m band
    } else if (muf > 21.0f) {
        return 15; // 15m band
    } else if (muf > 18.0f) {
        return 17; // 17m band
    } else if (muf > 14.0f) {
        return 20; // 20m band
    } else if (muf > 10.0f) {
        return 30; // 30m band
    } else if (muf > 7.0f) {
        return 40; // 40m band
    } else if (muf > 3.5f) {
        return 80; // 80m band
    } else {
        return 160; // 160m band
    }
}

float HFBandSimulation::calculateMUF(float distance) {
    // Calculate the critical frequency (foF2)
    float foF2 = calculateCriticalFrequency();
    
    // Calculate the F layer height
    float fLayerHeight = calculateFLayerHeight();
    
    // Calculate the maximum distance of single-hop propagation
    float maxDistance = 2.0f * std::sqrt(fLayerHeight * 2.0f * 6371.0f);
    
    // Calculate the number of hops required
    int hops = static_cast<int>(std::ceil(distance / maxDistance));
    
    // Calculate the take-off angle
    float takeoffAngle = std::atan(fLayerHeight / (distance / (2.0f * hops))) * 180.0f / PI;
    
    // Calculate the secant of the take-off angle
    float secant = 1.0f / std::cos(takeoffAngle * PI / 180.0f);
    
    // Calculate the MUF
    float muf = foF2 * secant;
    
    return muf;
}

void HFBandSimulation::updatePropagation() {
    // Update the season if auto time is enabled
    if (m_autoTimeEnabled) {
        updateSeason();
    }
    
    // Update external data if enabled
    if (m_useExternalData) {
        updateExternalData();
    }
    
    // Clear the signal strength cache
    m_signalStrengthCache.clear();
    
    // Emit the propagation updated signal
    emit propagationUpdated();
}

void HFBandSimulation::updateSeason() {
    // Get the current date
    QDate currentDate = QDate::currentDate();
    
    // Get the month
    int month = currentDate.month();
    
    // Determine the season based on the month
    // Northern hemisphere seasons:
    // Winter: December, January, February
    // Spring: March, April, May
    // Summer: June, July, August
    // Fall: September, October, November
    
    int newSeason;
    switch (month) {
        case 12:
        case 1:
        case 2:
            newSeason = 0; // Winter
            break;
        case 3:
        case 4:
        case 5:
            newSeason = 1; // Spring
            break;
        case 6:
        case 7:
        case 8:
            newSeason = 2; // Summer
            break;
        case 9:
        case 10:
        case 11:
            newSeason = 3; // Fall
            break;
        default:
            newSeason = 0; // Default to Winter
            break;
    }
    
    // Update the season
    setSeason(newSeason);
}

void HFBandSimulation::updateExternalData() {
    bool success = false;
    
    // Update DXView data
    if (m_useDXViewData) {
        success = updateDXViewData();
        emit externalDataUpdated("DXView.org", success);
    }
    
    // Update SWPC data
    if (m_useSWPCData) {
        success = updateSWPCData();
        emit externalDataUpdated("SWPC", success);
    }
}

bool HFBandSimulation::updateDXViewData() {
    // In a real implementation, this would fetch data from DXView.org
    // For this simplified version, we just return success
    
    // Simulate a successful update
    return true;
}

bool HFBandSimulation::updateSWPCData() {
    // In a real implementation, this would fetch data from SWPC
    // For this simplified version, we just return success
    
    // Simulate a successful update
    return true;
}

float HFBandSimulation::calculateCriticalFrequency() {
    // Calculate the critical frequency (foF2) based on various factors
    
    // 1. Base value
    float foF2 = 5.0f;
    
    // 2. Solar activity factor
    float solarFactor = 1.0f + (m_solarFluxIndex - 100.0f) / 100.0f;
    
    // 3. Geomagnetic activity factor (inversely proportional)
    float geomagneticFactor = 1.0f - (m_kIndex / 9.0f) * 0.5f;
    
    // 4. Season factor
    float seasonFactor = 1.0f;
    switch (m_season) {
        case 0: // Winter
            seasonFactor = 0.8f;
            break;
        case 1: // Spring
            seasonFactor = 1.0f;
            break;
        case 2: // Summer
            seasonFactor = 1.2f;
            break;
        case 3: // Fall
            seasonFactor = 1.0f;
            break;
        default:
            seasonFactor = 1.0f;
            break;
    }
    
    // Combine all factors
    foF2 *= solarFactor * geomagneticFactor * seasonFactor;
    
    return foF2;
}

float HFBandSimulation::calculateFLayerHeight() {
    // Calculate the F layer height based on various factors
    
    // 1. Base height
    float height = 300.0f;
    
    // 2. Solar activity factor
    float solarFactor = 1.0f + (m_solarFluxIndex - 100.0f) / 200.0f;
    
    // 3. Geomagnetic activity factor
    float geomagneticFactor = 1.0f + (m_kIndex / 9.0f) * 0.2f;
    
    // 4. Season factor
    float seasonFactor = 1.0f;
    switch (m_season) {
        case 0: // Winter
            seasonFactor = 1.1f;
            break;
        case 1: // Spring
            seasonFactor = 1.0f;
            break;
        case 2: // Summer
            seasonFactor = 0.9f;
            break;
        case 3: // Fall
            seasonFactor = 1.0f;
            break;
        default:
            seasonFactor = 1.0f;
            break;
    }
    
    // Combine all factors
    height *= solarFactor * geomagneticFactor * seasonFactor;
    
    return height;
}