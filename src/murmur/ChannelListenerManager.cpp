// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QReadLocker>
#include <QWriteLocker>

#include "ChannelListenerManager.h"
#include "Channel.h"
#include "User.h"
#include "VolumeAdjustment.h"

ChannelListenerManager::ChannelListenerManager(QObject *parent)
    : QObject(parent) {
}

ChannelListenerManager::~ChannelListenerManager() {
    clear();
}

void ChannelListenerManager::addListener(const ServerUser &user, const Channel &channel) {
    QWriteLocker locker(&m_rwLock);
    
    // Add to channel listeners
    m_channelListeners[&channel].insert(&user);
    
    // Add to user listened channels
    m_userListenedChannels[&user].insert(&channel);
    
    // Initialize with default volume adjustment if not set
    QPair<const ServerUser *, const Channel *> key(&user, &channel);
    if (!m_listenerVolumeAdjustments.contains(key)) {
        m_listenerVolumeAdjustments.insert(key, VolumeAdjustment());
    }
    
    locker.unlock();
    
    // Emit signal after releasing lock
    emit listenerAdded(user, channel);
}

void ChannelListenerManager::setListenerVolumeAdjustment(const ServerUser &user, const Channel &channel, const VolumeAdjustment &volumeAdjustment) {
    QWriteLocker locker(&m_rwLock);
    
    QPair<const ServerUser *, const Channel *> key(&user, &channel);
    m_listenerVolumeAdjustments.insert(key, volumeAdjustment);
    
    locker.unlock();
    
    // Emit signal after releasing lock
    emit listenerVolumeAdjustmentChanged(user, channel, volumeAdjustment);
}

VolumeAdjustment ChannelListenerManager::getListenerVolumeAdjustment(const ServerUser &user, const Channel &channel) const {
    QReadLocker locker(&m_rwLock);
    
    QPair<const ServerUser *, const Channel *> key(&user, &channel);
    return m_listenerVolumeAdjustments.value(key, VolumeAdjustment());
}

void ChannelListenerManager::disableListener(const ServerUser &user, const Channel &channel) {
    QWriteLocker locker(&m_rwLock);
    
    // Set volume to zero but keep the listener
    QPair<const ServerUser *, const Channel *> key(&user, &channel);
    VolumeAdjustment adjustment = m_listenerVolumeAdjustments.value(key, VolumeAdjustment());
    adjustment.setType(VolumeAdjustment::Multiplicative);
    adjustment.setAdjustmentFactor(&user, 0.0f);
    m_listenerVolumeAdjustments.insert(key, adjustment);
    
    locker.unlock();
    
    // Emit signal after releasing lock
    emit listenerVolumeAdjustmentChanged(user, channel, adjustment);
}

void ChannelListenerManager::removeListener(const ServerUser &user, const Channel &channel) {
    QWriteLocker locker(&m_rwLock);
    
    // Remove from channel listeners
    QSet<const ServerUser *> &listeners = m_channelListeners[&channel];
    listeners.remove(&user);
    
    // If no more listeners, remove the channel entry
    if (listeners.isEmpty()) {
        m_channelListeners.remove(&channel);
    }
    
    // Remove from user listened channels
    QSet<const Channel *> &channels = m_userListenedChannels[&user];
    channels.remove(&channel);
    
    // If no more channels, remove the user entry
    if (channels.isEmpty()) {
        m_userListenedChannels.remove(&user);
    }
    
    // Remove volume adjustment
    QPair<const ServerUser *, const Channel *> key(&user, &channel);
    m_listenerVolumeAdjustments.remove(key);
    
    locker.unlock();
    
    // Emit signal after releasing lock
    emit listenerRemoved(user, channel);
}

bool ChannelListenerManager::isListening(const ServerUser &user, const Channel &channel) const {
    QReadLocker locker(&m_rwLock);
    
    return m_channelListeners.contains(&channel) &&
           m_channelListeners.value(&channel).contains(&user);
}

QSet<const ServerUser *> ChannelListenerManager::getListeners(const Channel &channel) const {
    QReadLocker locker(&m_rwLock);
    
    return m_channelListeners.value(&channel);
}

QSet<const Channel *> ChannelListenerManager::getListenedChannels(const ServerUser &user) const {
    QReadLocker locker(&m_rwLock);
    
    return m_userListenedChannels.value(&user);
}

void ChannelListenerManager::clearListeners(const Channel &channel) {
    QWriteLocker locker(&m_rwLock);
    
    // Get listeners before clearing
    QSet<const ServerUser *> listeners = m_channelListeners.value(&channel);
    
    // Remove channel from all users' listened channels
    for (const ServerUser *user : listeners) {
        m_userListenedChannels[user].remove(&channel);
        
        // If no more channels, remove the user entry
        if (m_userListenedChannels[user].isEmpty()) {
            m_userListenedChannels.remove(user);
        }
        
        // Remove volume adjustment
        QPair<const ServerUser *, const Channel *> key(user, &channel);
        m_listenerVolumeAdjustments.remove(key);
    }
    
    // Remove channel entry
    m_channelListeners.remove(&channel);
    
    locker.unlock();
    
    // Emit signals after releasing lock
    for (const ServerUser *user : listeners) {
        emit listenerRemoved(*user, channel);
    }
}

void ChannelListenerManager::clearListenedChannels(const ServerUser &user) {
    QWriteLocker locker(&m_rwLock);
    
    // Get channels before clearing
    QSet<const Channel *> channels = m_userListenedChannels.value(&user);
    
    // Remove user from all channels' listeners
    for (const Channel *channel : channels) {
        m_channelListeners[channel].remove(&user);
        
        // If no more listeners, remove the channel entry
        if (m_channelListeners[channel].isEmpty()) {
            m_channelListeners.remove(channel);
        }
        
        // Remove volume adjustment
        QPair<const ServerUser *, const Channel *> key(&user, channel);
        m_listenerVolumeAdjustments.remove(key);
    }
    
    // Remove user entry
    m_userListenedChannels.remove(&user);
    
    locker.unlock();
    
    // Emit signals after releasing lock
    for (const Channel *channel : channels) {
        emit listenerRemoved(user, *channel);
    }
}

void ChannelListenerManager::clear() {
    QWriteLocker locker(&m_rwLock);
    
    m_channelListeners.clear();
    m_userListenedChannels.clear();
    m_listenerVolumeAdjustments.clear();
}