// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_USER_H_
#define MUMBLE_MURMUR_USER_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtCore/QByteArray>
#include <vector>

class Channel;
class ServerUser;
class Server;

/**
 * @brief Basic user information
 * 
 * Represents information about a user that's common
 * for both connected and registered users
 */
class UserInfo {
public:
    int userid;                 ///< User ID
    QString name;               ///< User name
    QString email;              ///< User email
    QByteArray pw;              ///< Password hash
    QDateTime lastChannelId;    ///< Last channel ID
    QDateTime lastActive;       ///< Last active timestamp
    
    /// Constructor
    UserInfo() : userid(-1) {}
    
    /// Get hash of the UserInfo for lookups
    uint32_t getHash() const;
    
    /// Equality operator
    bool operator==(const UserInfo &other) const;
    
    /// Get user name
    QString getName() const { return name; }
    
    /// Get user ID
    int getUserId() const { return userid; }
    
    /// Set user ID
    void setUserId(int id) { userid = id; }
    
    /// Set user name
    void setName(const QString &newName) { name = newName; }
};

/**
 * @brief Base User class
 * 
 * Represents a user in the Mumble server, which may be
 * either a connected client or a registered account
 */
class User {
public:
    int uiSession;              ///< Session ID
    int iId;                    ///< User ID
    QString qsName;             ///< Username
    QString qsComment;          ///< User comment/bio
    QByteArray qbaTexture;      ///< User avatar texture
    QMap<int, QString> qmTargets; ///< Whisper targets
    QMap<QString, QString> qmUserData; ///< User metadata
    
    /// Constructor
    User();
    
    /// Destructor
    virtual ~User();
    
    /// Get the hash of a user by name
    static uint qHash(const QString &name);
    
    /// Convert to string
    QString toString() const;
};

/**
 * @brief Extended user information for the server
 */
class ServerUserInfo : public UserInfo {
public:
    QDateTime created;          ///< Account creation date
    QDateTime lastConnect;      ///< Last connection date
    quint64 connectTime;        ///< Total connection time
    QString lastAddress;        ///< Last IP address
    
    /// Constructor
    ServerUserInfo() : UserInfo() {}
};

/**
 * @brief Connected user on the server
 * 
 * Represents a user currently connected to the server
 */
class ServerUser : public User {
public:
    Channel *cChannel;          ///< Current channel
    bool bMute, bDeaf, bSuppress; ///< Mute, deaf, suppress flags
    bool bSelfMute, bSelfDeaf;  ///< Self-mute, self-deaf flags
    bool bPrioritySpeaker;      ///< Priority speaker flag
    bool bRecording;            ///< Recording flag
    
    QList<Channel *> qlChannels; ///< Channels the user is in
    QMap<unsigned int, QString> qmWhispers; ///< Whisper targets
    
    QString qsGridSquare;       ///< HF band grid square location
    int iPower;                 ///< Transmitter power in watts
    QString qsAntennaType;      ///< Antenna type
    float fAntennaGain;         ///< Antenna gain in dBi
    QString qsFrequency;        ///< Operating frequency
    
    /// Constructor
    ServerUser(Server *parent, QByteArray certHash = QByteArray());
    
    /// Destructor
    ~ServerUser() override;
    
    /// Get transmission quality score (0-100)
    int getTransmissionQuality() const;
    
    /// Calculate propagation loss between this user and another
    float calculatePropagationLoss(ServerUser *target) const;
    
    /// Check if user has a valid grid square set
    bool hasValidGridSquare() const;
    
    /// Set grid square location
    void setGridSquare(const QString &grid);
    
    /// Set transmitter power
    void setPower(int watts);
    
    /// Set antenna type
    void setAntennaType(const QString &type);
    
    /// Set antenna gain
    void setAntennaGain(float dBi);
    
    /// Set operating frequency
    void setFrequency(const QString &freq);
};

/**
 * @brief Hash function for User objects
 * @param user User to hash
 * @return Hash value
 */
uint qHash(const User &user);

#endif // MUMBLE_MURMUR_USER_H_