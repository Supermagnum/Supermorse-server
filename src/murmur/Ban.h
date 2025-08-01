// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_BAN_H_
#define MUMBLE_MURMUR_BAN_H_

#include <QDateTime>
#include <QString>
#include "HostAddress.h"

// Ban structure for Mumble server
struct Ban {
    /**
     * @brief Default constructor
     */
    Ban() = default;
    
    /**
     * @brief Constructor with parameters
     * 
     * @param address IP address of the banned user
     * @param mask Network mask
     * @param name Username of the banned user
     * @param hash Certificate hash
     * @param reason Reason for the ban
     * @param start Start time of the ban
     * @param duration Duration of the ban in seconds (0 = permanent)
     */
    Ban(const HostAddress &address, int mask, const QString &name, 
        const QString &hash, const QString &reason, 
        const QDateTime &start, unsigned int duration);
    
    /**
     * @brief Check if the ban is valid
     * 
     * @return true if the ban is valid
     */
    bool isValid() const;
    
    /**
     * @brief Check if the ban has expired
     * 
     * @return true if the ban has expired
     */
    bool isExpired() const;
    
    /**
     * @brief Get the remaining time of the ban
     * 
     * @return Remaining time in seconds, or 0 if permanent
     */
    unsigned int getRemainingTime() const;
    
    /**
     * @brief Get the expiration time of the ban
     * 
     * @return Expiration time, or an invalid date for permanent bans
     */
    QDateTime getExpirationTime() const;
    
    // Member variables
    HostAddress address; // IP address
    int mask = 0;        // Network mask
    QString name;        // Username
    QString hash;        // Certificate hash
    QString reason;      // Reason for the ban
    QDateTime start;     // Start time of the ban
    unsigned int duration = 0; // Duration in seconds (0 = permanent)
};

#endif // MUMBLE_MURMUR_BAN_H_