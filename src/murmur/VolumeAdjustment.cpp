// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeAdjustment.h"
#include "User.h"
#include "Server.h"
#include <cmath>
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <random>

// Constants for volume adjustment
const float MIN_ADJUSTMENT_FACTOR = 0.0f;
const float MAX_ADJUSTMENT_FACTOR = 10.0f;
const float DEFAULT_ADJUSTMENT_FACTOR = 1.0f;

/**
 * Constructor for VolumeAdjustment
 */
VolumeAdjustment::VolumeAdjustment(AdjustmentType type)
    : m_type(type), m_defaultFactor(DEFAULT_ADJUSTMENT_FACTOR) {
}

/**
 * Destructor for VolumeAdjustment
 */
VolumeAdjustment::~VolumeAdjustment() {
}

/**
 * Get the adjustment type
 */
VolumeAdjustment::AdjustmentType VolumeAdjustment::getType() const {
    return m_type;
}

/**
 * Set the adjustment type
 */
void VolumeAdjustment::setType(AdjustmentType type) {
    m_type = type;
}

/**
 * Apply volume adjustment to a sample
 */
float VolumeAdjustment::adjustSample(float sample, float factor) const {
    switch (m_type) {
        case Normal:
            return sample * factor;
        case Multiplicative:
            return sample * factor;
        case Logarithmic:
            return sample * qPow(2.0f, factor);
        case UserSpecific:
            return sample * factor;
        default:
            return sample;
    }
}

/**
 * Get adjustment factor for a user
 */
float VolumeAdjustment::getAdjustmentFactor(const User* user) const {
    if (!user) {
        return m_defaultFactor;
    }
    
    auto it = m_factors.find(user);
    if (it != m_factors.end()) {
        return it.value();
    }
    
    return m_defaultFactor;
}

/**
 * Set adjustment factor for a user
 */
void VolumeAdjustment::setAdjustmentFactor(const User* user, float factor) {
    if (!user) {
        m_defaultFactor = qBound(MIN_ADJUSTMENT_FACTOR, factor, MAX_ADJUSTMENT_FACTOR);
        return;
    }
    
    m_factors[user] = qBound(MIN_ADJUSTMENT_FACTOR, factor, MAX_ADJUSTMENT_FACTOR);
}

/**
 * Apply a user-specific volume adjustment
 */
float VolumeAdjustment::adjustForUser(float sample, const User* user) const {
    return adjustSample(sample, getAdjustmentFactor(user));
}

/**
 * Get minimum adjustment factor
 */
float VolumeAdjustment::getMinAdjustmentFactor() {
    return MIN_ADJUSTMENT_FACTOR;
}

/**
 * Get maximum adjustment factor
 */
float VolumeAdjustment::getMaxAdjustmentFactor() {
    return MAX_ADJUSTMENT_FACTOR;
}

/**
 * Convert decibels to linear factor
 */
float VolumeAdjustment::dBToFactor(float dB) {
    return qPow(10.0f, dB / 20.0f);
}

/**
 * Convert linear factor to decibels
 */
float VolumeAdjustment::factorTodB(float factor) {
    if (factor <= 0.0f) {
        return -INFINITY;
    }
    return 20.0f * log10(factor);
}

// SuperMorseVolumeAdjustment implementation

/**
 * Constructor for SuperMorseVolumeAdjustment
 */
SuperMorseVolumeAdjustment::SuperMorseVolumeAdjustment()
    : VolumeAdjustment(Normal), m_propagationEnabled(true), m_solarFlux(100.0f), m_kIndex(3), m_noiseFloor(0.01f) {
}

/**
 * Destructor for SuperMorseVolumeAdjustment
 */
SuperMorseVolumeAdjustment::~SuperMorseVolumeAdjustment() {
}

/**
 * Get adjustment factor for a server user
 */
float SuperMorseVolumeAdjustment::getAdjustmentFactor(const User* user) const {
    float baseFactor = VolumeAdjustment::getAdjustmentFactor(user);
    
    if (!m_propagationEnabled || !user) {
        return baseFactor;
    }
    
    // Additional adjustments can be made based on propagation conditions
    return baseFactor;
}

/**
 * Apply HF propagation effects to audio
 */
float SuperMorseVolumeAdjustment::applyPropagationEffects(float sample, const ServerUser* sourceUser, const ServerUser* targetUser) const {
    if (!m_propagationEnabled || !sourceUser || !targetUser) {
        return sample;
    }
    
    int signalQuality = calculateSignalQuality(sourceUser, targetUser);
    
    // Apply fading and static based on signal quality
    sample = applyFading(sample, 1.0f - (signalQuality / 100.0f));
    sample = applyStatic(sample, signalQuality);
    
    return sample;
}

/**
 * Apply static based on signal quality
 */
float SuperMorseVolumeAdjustment::applyStatic(float sample, int signalQuality) const {
    if (signalQuality >= 95) {
        return sample; // No static for excellent signals
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(-1.0, 1.0);
    
    // Calculate static intensity based on signal quality
    float staticIntensity = 1.0f - (signalQuality / 100.0f);
    staticIntensity = qPow(staticIntensity, 2.0f); // Make it non-linear
    
    // Add noise proportional to static intensity
    float noise = static_cast<float>(dis(gen)) * staticIntensity * 0.3f;
    
    return sample * (1.0f - staticIntensity) + noise;
}

/**
 * Apply fading based on propagation conditions
 */
float SuperMorseVolumeAdjustment::applyFading(float sample, float fadeLevel) const {
    if (fadeLevel < 0.05f) {
        return sample; // No significant fading
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    
    // Apply random amplitude modulation to simulate fading
    float fadeModulation = 1.0f - (fadeLevel * 0.5f * static_cast<float>(dis(gen)));
    
    return sample * fadeModulation;
}

/**
 * Calculate signal quality between two users
 */
int SuperMorseVolumeAdjustment::calculateSignalQuality(const ServerUser* sourceUser, const ServerUser* targetUser) const {
    if (!sourceUser || !targetUser) {
        return 0;
    }
    
    // Base quality depending on conditions
    int baseQuality = 95 - (m_kIndex * 5);
    
    // Adjust based on solar flux (higher flux = better HF propagation)
    float solarFluxFactor = qBound(0.8f, m_solarFlux / 150.0f, 1.2f);
    baseQuality = static_cast<int>(baseQuality * solarFluxFactor);
    
    // Add some variability
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(-5, 5);
    
    return qBound(20, baseQuality + dis(gen), 100);
}

/**
 * Set current ionospheric conditions
 */
void SuperMorseVolumeAdjustment::setIonosphericConditions(float solarFlux, int kIndex) {
    m_solarFlux = qBound(50.0f, solarFlux, 300.0f);
    m_kIndex = qBound(0, kIndex, 9);
}

/**
 * Enable or disable propagation simulation
 */
void SuperMorseVolumeAdjustment::setPropagationEnabled(bool enabled) {
    m_propagationEnabled = enabled;
}

/**
 * Check if propagation simulation is enabled
 */
bool SuperMorseVolumeAdjustment::isPropagationEnabled() const {
    return m_propagationEnabled;
}