// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_
#define MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_

#include <QHash>
#include <QObject>
#include <QSet>
#include <QVector>

class ServerUser;
class VolumeAdjustment;

// Audio Receiver Buffer for Mumble server
class AudioReceiverBuffer {
public:
    /**
     * @brief Constructor for AudioReceiverBuffer
     */
    AudioReceiverBuffer();
    
    /**
     * @brief Destructor for AudioReceiverBuffer
     */
    ~AudioReceiverBuffer();
    
    /**
     * @brief Add a user to receive audio from the specified speaker
     * 
     * @param speaker User sending audio
     * @param receiver User receiving audio
     * @param volumeAdjustment Volume adjustment to apply
     */
    void addReceiver(ServerUser *speaker, ServerUser *receiver, const VolumeAdjustment &volumeAdjustment);
    
    /**
     * @brief Get all receivers for a speaker
     * 
     * @param speaker User sending audio
     * @return QHash mapping receivers to their volume adjustments
     */
    QHash<ServerUser *, VolumeAdjustment> getReceivers(ServerUser *speaker) const;
    
    /**
     * @brief Remove a receiver for a speaker
     * 
     * @param speaker User sending audio
     * @param receiver User receiving audio
     */
    void removeReceiver(ServerUser *speaker, ServerUser *receiver);
    
    /**
     * @brief Remove all receivers for a speaker
     * 
     * @param speaker User sending audio
     */
    void removeReceivers(ServerUser *speaker);
    
    /**
     * @brief Clear all receivers from the buffer
     */
    void clear();
    
    /**
     * @brief Check if a user is receiving audio from a speaker
     * 
     * @param speaker User sending audio
     * @param receiver User potentially receiving audio
     * @return true if the receiver is receiving from the speaker
     */
    bool isReceiving(ServerUser *speaker, ServerUser *receiver) const;
    
    /**
     * @brief Update volume adjustment for a receiver
     * 
     * @param speaker User sending audio
     * @param receiver User receiving audio
     * @param volumeAdjustment New volume adjustment
     */
    void updateVolumeAdjustment(ServerUser *speaker, ServerUser *receiver, const VolumeAdjustment &volumeAdjustment);
    
private:
    // Map of speakers to their receivers and volume adjustments
    QHash<ServerUser *, QHash<ServerUser *, VolumeAdjustment>> m_speakerReceivers;
};

#endif // MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_