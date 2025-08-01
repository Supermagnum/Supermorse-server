// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QMutexLocker>
#include <QReadLocker>
#include <QWriteLocker>

#include "AudioReceiverBuffer.h"
#include "User.h"
#include "VolumeAdjustment.h"

AudioReceiverBuffer::AudioReceiverBuffer() {
}

AudioReceiverBuffer::~AudioReceiverBuffer() {
    clear();
}

void AudioReceiverBuffer::addReceiver(ServerUser *speaker, ServerUser *receiver, const VolumeAdjustment &volumeAdjustment) {
    if (!speaker || !receiver) {
        return;
    }
    
    // Add or update the receiver for this speaker
    m_speakerReceivers[speaker][receiver] = volumeAdjustment;
}

QHash<ServerUser *, VolumeAdjustment> AudioReceiverBuffer::getReceivers(ServerUser *speaker) const {
    if (!speaker) {
        return QHash<ServerUser *, VolumeAdjustment>();
    }
    
    return m_speakerReceivers.value(speaker);
}

void AudioReceiverBuffer::removeReceiver(ServerUser *speaker, ServerUser *receiver) {
    if (!speaker || !receiver) {
        return;
    }
    
    if (m_speakerReceivers.contains(speaker)) {
        m_speakerReceivers[speaker].remove(receiver);
        
        // If this was the last receiver for this speaker, remove the speaker entry
        if (m_speakerReceivers[speaker].isEmpty()) {
            m_speakerReceivers.remove(speaker);
        }
    }
}

void AudioReceiverBuffer::removeReceivers(ServerUser *speaker) {
    if (!speaker) {
        return;
    }
    
    m_speakerReceivers.remove(speaker);
}

void AudioReceiverBuffer::clear() {
    m_speakerReceivers.clear();
}

bool AudioReceiverBuffer::isReceiving(ServerUser *speaker, ServerUser *receiver) const {
    if (!speaker || !receiver) {
        return false;
    }
    
    return m_speakerReceivers.contains(speaker) && 
           m_speakerReceivers.value(speaker).contains(receiver);
}

void AudioReceiverBuffer::updateVolumeAdjustment(ServerUser *speaker, ServerUser *receiver, const VolumeAdjustment &volumeAdjustment) {
    if (!speaker || !receiver) {
        return;
    }
    
    if (m_speakerReceivers.contains(speaker) && m_speakerReceivers[speaker].contains(receiver)) {
        m_speakerReceivers[speaker][receiver] = volumeAdjustment;
    }
}