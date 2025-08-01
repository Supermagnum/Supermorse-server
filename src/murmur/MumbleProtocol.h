// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_MUMBLEPROTOCOL_H_
#define MUMBLE_MURMUR_MUMBLEPROTOCOL_H_

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>
#include <QVector>

#include <cstdint>

namespace Mumble {
namespace Protocol {

// Basic type for protocol byte representation
typedef unsigned char byte;

// Protocol version
const uint32_t PROTOCOL_VERSION = 0x10205;

// Roles in the protocol
enum class Role {
    Client,
    Server
};

// TCP Message Types
enum class TCPMessageType {
    Version = 0,
    UDPTunnel = 1,
    Authenticate = 2,
    Ping = 3,
    Reject = 4,
    ServerSync = 5,
    ChannelRemove = 6,
    ChannelState = 7,
    UserRemove = 8,
    UserState = 9,
    BanList = 10,
    TextMessage = 11,
    PermissionDenied = 12,
    ACL = 13,
    QueryUsers = 14,
    CryptSetup = 15,
    ContextActionModify = 16,
    ContextAction = 17,
    UserList = 18,
    VoiceTarget = 19,
    PermissionQuery = 20,
    CodecVersion = 21,
    UserStats = 22,
    RequestBlob = 23,
    ServerConfig = 24,
    SuggestConfig = 25,
    PluginDataTransmission = 26,
    ChannelListener = 27,
    HFBandSimulationUpdate = 28,
    SignalQualityUpdate = 29,
    PropagationUpdate = 30
};

// UDP Message Types
enum class UDPMessageType {
    VoiceData = 0,
    Ping = 1,
    VoiceOpus = 4
};

// Audio data structure
struct AudioData {
    byte *data;
    int size;
    int frameSize;
    bool isOpus;
    uint32_t senderSession;
    QList<uint32_t> targetSessions;
    
    AudioData() : data(nullptr), size(0), frameSize(0), isOpus(true), senderSession(0) {}
    
    ~AudioData() {
        if (data) {
            delete[] data;
            data = nullptr;
        }
    }
};

// Protocol decoder for UDP packets
template<Role R>
class UDPDecoder {
public:
    UDPDecoder() : m_valid(false), m_type(UDPMessageType::Ping) {}
    
    bool decode(const byte *buffer, int length) {
        if (length < 1) {
            m_valid = false;
            return false;
        }
        
        // In a real implementation, this would decode the packet
        // For this simplified version, just set some basic properties
        m_valid = true;
        m_type = static_cast<UDPMessageType>(buffer[0] & 0x7);
        return true;
    }
    
    bool isValid() const { return m_valid; }
    UDPMessageType getType() const { return m_type; }
    
private:
    bool m_valid;
    UDPMessageType m_type;
};

// Protocol encoder for UDP ping packets
template<Role R>
class UDPPingEncoder {
public:
    UDPPingEncoder() {}
    
    int encode(byte *buffer, int length, uint64_t timestamp) {
        if (length < 9) {
            return 0;
        }
        
        // In a real implementation, this would encode the packet
        // For this simplified version, just set some basic properties
        buffer[0] = static_cast<byte>(UDPMessageType::Ping);
        
        // Encode timestamp (8 bytes)
        for (int i = 0; i < 8; i++) {
            buffer[i + 1] = (timestamp >> (i * 8)) & 0xFF;
        }
        
        return 9;
    }
};

// Protocol encoder for UDP audio packets
template<Role R>
class UDPAudioEncoder {
public:
    UDPAudioEncoder() {}
    
    int encode(byte *buffer, int length, const AudioData &audioData) {
        if (length < audioData.size + 1 || !audioData.data) {
            return 0;
        }
        
        // In a real implementation, this would encode the packet
        // For this simplified version, just set some basic properties
        buffer[0] = static_cast<byte>(
            audioData.isOpus ? UDPMessageType::VoiceOpus : UDPMessageType::VoiceData
        );
        
        // Copy audio data
        memcpy(buffer + 1, audioData.data, audioData.size);
        
        return audioData.size + 1;
    }
};

// Protocol handler for TCP packets (wrapper around protobuf messages)
class TCPMessageHandler {
public:
    TCPMessageHandler() {}
    
    QByteArray encodeMessage(TCPMessageType type, const QByteArray &message) {
        QByteArray packet;
        
        // In a real implementation, this would encode the packet
        // For this simplified version, create a simple header and append the message
        
        // 2 bytes for message type
        packet.append(static_cast<char>((static_cast<int>(type) >> 8) & 0xFF));
        packet.append(static_cast<char>(static_cast<int>(type) & 0xFF));
        
        // 4 bytes for message length
        uint32_t length = message.size();
        packet.append(static_cast<char>((length >> 24) & 0xFF));
        packet.append(static_cast<char>((length >> 16) & 0xFF));
        packet.append(static_cast<char>((length >> 8) & 0xFF));
        packet.append(static_cast<char>(length & 0xFF));
        
        // Message content
        packet.append(message);
        
        return packet;
    }
    
    bool decodeMessageHeader(const QByteArray &data, TCPMessageType &type, uint32_t &length) {
        if (data.size() < 6) {
            return false;
        }
        
        // Extract message type (2 bytes)
        int typeValue = ((static_cast<unsigned char>(data[0]) << 8) | 
                         (static_cast<unsigned char>(data[1])));
        type = static_cast<TCPMessageType>(typeValue);
        
        // Extract message length (4 bytes)
        length = ((static_cast<unsigned char>(data[2]) << 24) |
                 (static_cast<unsigned char>(data[3]) << 16) |
                 (static_cast<unsigned char>(data[4]) << 8) |
                 (static_cast<unsigned char>(data[5])));
        
        return true;
    }
};

} // namespace Protocol
} // namespace Mumble

#endif // MUMBLE_MURMUR_MUMBLEPROTOCOL_H_