// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_VOLUMEADJUSTMENT_H_
#define MUMBLE_MURMUR_VOLUMEADJUSTMENT_H_

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <cmath>

class User;
class ServerUser;

/**
 * @brief The VolumeAdjustment class handles audio volume modifications
 * 
 * This class provides functionality for adjusting audio volume levels
 * for different users and channels in the Mumble server.
 */
class VolumeAdjustment {
public:
    /**
     * @brief Types of volume adjustments
     */
    enum AdjustmentType {
        Normal,         ///< Normal volume adjustment
        Multiplicative, ///< Multiplicative scaling
        Logarithmic,    ///< Logarithmic scaling
        UserSpecific    ///< User-specific adjustment
    };

    /**
     * @brief Constructor
     * @param type Adjustment type
     */
    VolumeAdjustment(AdjustmentType type = Normal);
    
    /**
     * @brief Destructor
     */
    virtual ~VolumeAdjustment();
    
    /**
     * @brief Get adjustment type
     * @return The adjustment type
     */
    AdjustmentType getType() const;
    
    /**
     * @brief Set adjustment type
     * @param type New adjustment type
     */
    void setType(AdjustmentType type);
    
    /**
     * @brief Apply volume adjustment to a sample
     * @param sample Audio sample to adjust
     * @param factor Adjustment factor
     * @return Adjusted sample
     */
    virtual float adjustSample(float sample, float factor) const;
    
    /**
     * @brief Get adjustment factor for a user
     * @param user User to get adjustment for
     * @return Adjustment factor
     */
    virtual float getAdjustmentFactor(const User* user) const;
    
    /**
     * @brief Set adjustment factor for a user
     * @param user User to set adjustment for
     * @param factor New adjustment factor
     */
    virtual void setAdjustmentFactor(const User* user, float factor);
    
    /**
     * @brief Apply a user-specific volume adjustment
     * @param sample Audio sample to adjust
     * @param user User to apply adjustment for
     * @return Adjusted sample
     */
    float adjustForUser(float sample, const User* user) const;
    
    /**
     * @brief Get minimum adjustment factor
     * @return Minimum adjustment factor
     */
    static float getMinAdjustmentFactor();
    
    /**
     * @brief Get maximum adjustment factor
     * @return Maximum adjustment factor
     */
    static float getMaxAdjustmentFactor();
    
    /**
     * @brief Convert decibels to linear factor
     * @param dB Decibel value
     * @return Linear factor
     */
    static float dBToFactor(float dB);
    
    /**
     * @brief Convert linear factor to decibels
     * @param factor Linear factor
     * @return Decibel value
     */
    static float factorTodB(float factor);

protected:
    AdjustmentType m_type;              ///< Adjustment type
    QMap<const User*, float> m_factors; ///< User-specific adjustment factors
    float m_defaultFactor;              ///< Default adjustment factor
};

/**
 * @brief The SuperMorseVolumeAdjustment class adds HF band-specific volume adjustments
 * 
 * This class extends VolumeAdjustment with features specific to HF band
 * simulation, like signal quality based on propagation conditions.
 */
class SuperMorseVolumeAdjustment : public VolumeAdjustment {
public:
    /**
     * @brief Constructor
     */
    SuperMorseVolumeAdjustment();
    
    /**
     * @brief Destructor
     */
    ~SuperMorseVolumeAdjustment() override;
    
    /**
     * @brief Get adjustment factor for a server user
     * @param user Server user to get adjustment for
     * @return Adjustment factor based on propagation conditions
     */
    float getAdjustmentFactor(const User* user) const override;
    
    /**
     * @brief Apply HF propagation effects to audio
     * @param sample Audio sample
     * @param sourceUser Source user
     * @param targetUser Target user
     * @return Adjusted sample with propagation effects
     */
    float applyPropagationEffects(float sample, const ServerUser* sourceUser, const ServerUser* targetUser) const;
    
    /**
     * @brief Apply static based on signal quality
     * @param sample Audio sample
     * @param signalQuality Signal quality (0-100)
     * @return Sample with static applied
     */
    float applyStatic(float sample, int signalQuality) const;
    
    /**
     * @brief Apply fading based on propagation conditions
     * @param sample Audio sample
     * @param fadeLevel Fade level (0-1)
     * @return Sample with fading applied
     */
    float applyFading(float sample, float fadeLevel) const;
    
    /**
     * @brief Calculate signal quality between two users
     * @param sourceUser Source user
     * @param targetUser Target user
     * @return Signal quality (0-100)
     */
    int calculateSignalQuality(const ServerUser* sourceUser, const ServerUser* targetUser) const;
    
    /**
     * @brief Set current ionospheric conditions
     * @param solarFlux Solar flux index
     * @param kIndex K-index
     */
    void setIonosphericConditions(float solarFlux, int kIndex);
    
    /**
     * @brief Enable or disable propagation simulation
     * @param enabled Whether propagation simulation is enabled
     */
    void setPropagationEnabled(bool enabled);
    
    /**
     * @brief Check if propagation simulation is enabled
     * @return True if propagation simulation is enabled
     */
    bool isPropagationEnabled() const;

private:
    bool m_propagationEnabled;   ///< Whether propagation simulation is enabled
    float m_solarFlux;           ///< Current solar flux index
    int m_kIndex;                ///< Current K-index
    float m_noiseFloor;          ///< Current noise floor level
};

#endif // MUMBLE_MURMUR_VOLUMEADJUSTMENT_H_