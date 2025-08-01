// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_CHANNELLISTENERMANAGER_H_
#define MUMBLE_MURMUR_CHANNELLISTENERMANAGER_H_

#include <QHash>
#include <QObject>
#include <QSet>
#include <QReadWriteLock>

class Channel;
class ServerUser;
class VolumeAdjustment;

/**
 * @brief The ChannelListenerManager class manages listeners in channels.
 * 
 * This class tracks which users are listening to which channels,
 * and manages volume adjustments for listeners.
 */
class ChannelListenerManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for ChannelListenerManager
     * @param parent The parent QObject
     */
    explicit ChannelListenerManager(QObject *parent = nullptr);
    
    /**
     * @brief Destructor for ChannelListenerManager
     */
    ~ChannelListenerManager();
    
    /**
     * @brief Add a listener to a channel
     * 
     * @param user The user to add as a listener
     * @param channel The channel to listen to
     */
    void addListener(const ServerUser &user, const Channel &channel);
    
    /**
     * @brief Set volume adjustment for a listener
     * 
     * @param user The listener user
     * @param channel The channel being listened to
     * @param volumeAdjustment The volume adjustment to apply
     */
    void setListenerVolumeAdjustment(const ServerUser &user, const Channel &channel, const VolumeAdjustment &volumeAdjustment);
    
    /**
     * @brief Get volume adjustment for a listener
     * 
     * @param user The listener user
     * @param channel The channel being listened to
     * @return The volume adjustment for the listener
     */
    VolumeAdjustment getListenerVolumeAdjustment(const ServerUser &user, const Channel &channel) const;
    
    /**
     * @brief Disable a channel listener
     * 
     * @param user The listener user
     * @param channel The channel being listened to
     */
    void disableListener(const ServerUser &user, const Channel &channel);
    
    /**
     * @brief Remove a listener from a channel
     * 
     * @param user The listener user
     * @param channel The channel being listened to
     */
    void removeListener(const ServerUser &user, const Channel &channel);
    
    /**
     * @brief Check if a user is listening to a channel
     * 
     * @param user The user to check
     * @param channel The channel to check
     * @return true if the user is listening to the channel
     */
    bool isListening(const ServerUser &user, const Channel &channel) const;
    
    /**
     * @brief Get all listeners for a channel
     * 
     * @param channel The channel to get listeners for
     * @return QSet of users listening to the channel
     */
    QSet<const ServerUser *> getListeners(const Channel &channel) const;
    
    /**
     * @brief Get all channels a user is listening to
     * 
     * @param user The user to get channels for
     * @return QSet of channels the user is listening to
     */
    QSet<const Channel *> getListenedChannels(const ServerUser &user) const;
    
    /**
     * @brief Clear all listeners for a channel
     * 
     * @param channel The channel to clear listeners for
     */
    void clearListeners(const Channel &channel);
    
    /**
     * @brief Clear all channels a user is listening to
     * 
     * @param user The user to clear channels for
     */
    void clearListenedChannels(const ServerUser &user);
    
    /**
     * @brief Clear all listeners and channels
     */
    void clear();
    
signals:
    /**
     * @brief Signal emitted when a user starts listening to a channel
     * 
     * @param user The user listening
     * @param channel The channel being listened to
     */
    void listenerAdded(const ServerUser &user, const Channel &channel);
    
    /**
     * @brief Signal emitted when a user stops listening to a channel
     * 
     * @param user The user who was listening
     * @param channel The channel that was being listened to
     */
    void listenerRemoved(const ServerUser &user, const Channel &channel);
    
    /**
     * @brief Signal emitted when a listener's volume adjustment changes
     * 
     * @param user The listener user
     * @param channel The channel being listened to
     * @param volumeAdjustment The new volume adjustment
     */
    void listenerVolumeAdjustmentChanged(const ServerUser &user, const Channel &channel, const VolumeAdjustment &volumeAdjustment);
    
private:
    // Maps channels to their listeners
    QHash<const Channel *, QSet<const ServerUser *>> m_channelListeners;
    
    // Maps users to the channels they're listening to
    QHash<const ServerUser *, QSet<const Channel *>> m_userListenedChannels;
    
    // Maps (user, channel) pairs to their volume adjustments
    QHash<QPair<const ServerUser *, const Channel *>, VolumeAdjustment> m_listenerVolumeAdjustments;
    
    // Lock for thread safety
    mutable QReadWriteLock m_rwLock;
};

#endif // MUMBLE_MURMUR_CHANNELLISTENERMANAGER_H_