// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_WHISPERTARGET_H_
#define MUMBLE_MURMUR_WHISPERTARGET_H_

#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QString>

class ServerUser;
class Channel;

/**
 * @brief The WhisperTarget class represents a target for a whisper message
 * 
 * A whisper target can be a list of session IDs (direct targets),
 * a list of channel IDs (channel targets), or a list of channel links.
 */
class WhisperTarget {
public:
    WhisperTarget() = default;
    
    /**
     * @brief Add a direct target user session ID
     * 
     * @param session The session ID to add
     */
    void addSession(unsigned int session);
    
    /**
     * @brief Add a channel target
     * 
     * @param channel The channel ID to add
     * @param recursive Whether to include all subchannels
     */
    void addChannel(unsigned int channel, bool recursive = false);
    
    /**
     * @brief Check if the target is valid
     * 
     * @return true if the target is valid
     */
    bool isValid() const;
    
    /**
     * @brief Get the list of session IDs
     * 
     * @return List of session IDs
     */
    const QList<unsigned int>& getSessions() const;
    
    /**
     * @brief Get the list of channel targets
     * 
     * @return List of channel IDs
     */
    const QList<unsigned int>& getChannels() const;
    
    /**
     * @brief Get whether channels should be processed recursively
     * 
     * @return Whether to include subchannels
     */
    bool isRecursive() const;

private:
    QList<unsigned int> m_sessions;  // List of user session IDs
    QList<unsigned int> m_channels;  // List of channel IDs
    bool m_recursive = false;        // Whether to include subchannels
};

/**
 * @brief The WhisperTargetCache class caches whisper target information
 * 
 * This class is used to optimize whisper target resolution by caching
 * the list of users that should receive a whisper message based on
 * the original target specification.
 */
class WhisperTargetCache {
public:
    WhisperTargetCache() = default;
    
    /**
     * @brief Add a user to the target cache
     * 
     * @param user The user to add
     */
    void addUser(ServerUser* user);
    
    /**
     * @brief Get the list of users in the cache
     * 
     * @return List of users
     */
    const QSet<ServerUser*>& getUsers() const;
    
    /**
     * @brief Check if the cache is valid
     * 
     * @return true if the cache is valid
     */
    bool isValid() const;
    
    /**
     * @brief Clear the cache
     */
    void clear();

private:
    QSet<ServerUser*> m_users;  // Set of users in the target
    bool m_valid = false;       // Whether the cache is valid
};

#endif // MUMBLE_MURMUR_WHISPERTARGET_H_