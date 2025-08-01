// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_MUMBLE_PB_H_
#define MUMBLE_MURMUR_MUMBLE_PB_H_

#include <QByteArray>
#include <QList>
#include <QString>
#include <QFlags>
#include <cstdint>
#include <google/protobuf/message.h>

// Mumble Protocol Buffers definitions
namespace MumbleProto {

// Forward declarations of all message types
class Version;
class UDPTunnel;
class Authenticate;
class Ping;
class Reject;
class ServerSync;
class ChannelRemove;
class ChannelState;
class UserRemove;
class UserState;
class BanList;
class TextMessage;
class PermissionDenied;
class ACL;
class QueryUsers;
class CryptSetup;
class ContextActionModify;
class ContextAction;
class UserList;
class VoiceTarget;
class PermissionQuery;
class CodecVersion;
class UserStats;
class RequestBlob;
class ServerConfig;
class SuggestConfig;
class PluginData;
class UserIdentity;

// Base message type for all protocol messages - inherits from google::protobuf::Message
class Message : public google::protobuf::Message {
public:
    virtual ~Message() {}
    
    // Protocol message serialization methods
    virtual QByteArray SerializeAsString() const = 0;
    virtual bool ParseFromString(const QByteArray &data) = 0;
    virtual void Clear() = 0;
    virtual bool IsInitialized() const = 0;
    
    // Implementation of required google::protobuf::Message methods
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
};
// Custom Supermorse message types
class PluginDataTransmission : public Message {
public:
    PluginDataTransmission() : sender(0), receiver(0), data_id(0) {}
    
    uint32_t sender;
    uint32_t receiver;
    uint32_t data_id;
    std::string plugin_id;
    QByteArray data;
    
    QByteArray SerializeAsString() const override { return QByteArray(); }
    bool ParseFromString(const QByteArray &) override { return true; }
    void Clear() override {}
    bool IsInitialized() const override { return true; }
    
    void set_sender(uint32_t s) { sender = s; }
    void set_receiver(uint32_t r) { receiver = r; }
    void set_data_id(uint32_t id) { data_id = id; }
    void set_plugin_id(const std::string &id) { plugin_id = id; }
    void set_data(const QByteArray &d) { data = d; }
};

class ChannelListener : public Message {
public:
    ChannelListener() : session(0), channel_id(0), enabled(false), volume(1.0f) {}
    
    uint32_t session;
    uint32_t channel_id;
    bool enabled;
    float volume;
    
    QByteArray SerializeAsString() const override { return QByteArray(); }
    bool ParseFromString(const QByteArray &) override { return true; }
    void Clear() override {}
    bool IsInitialized() const override { return true; }
    
    void set_session(uint32_t s) { session = s; }
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_enabled(bool e) { enabled = e; }
    void set_volume(float v) { volume = v; }
};

class HFBandSimulationUpdate : public Message {
public:
    HFBandSimulationUpdate() : frequency(0), band_conditions(0), time_of_day(0) {}
    
    uint32_t frequency;
    uint32_t band_conditions;
    uint32_t time_of_day;
    QList<std::string> active_regions;
    
    QByteArray SerializeAsString() const override { return QByteArray(); }
    bool ParseFromString(const QByteArray &) override { return true; }
    void Clear() override {}
    bool IsInitialized() const override { return true; }
    
    void set_frequency(uint32_t f) { frequency = f; }
    void set_band_conditions(uint32_t c) { band_conditions = c; }
    void set_time_of_day(uint32_t t) { time_of_day = t; }
    void add_active_regions(const std::string &r) { active_regions.append(r); }
};

class SignalQualityUpdate : public Message {
public:
    SignalQualityUpdate() : session(0), signal_strength(0), noise_level(0), snr(0) {}
    
    uint32_t session;
    float signal_strength;
    float noise_level;
    float snr;
    std::string grid_square;
    
    QByteArray SerializeAsString() const override { return QByteArray(); }
    bool ParseFromString(const QByteArray &) override { return true; }
    void Clear() override {}
    bool IsInitialized() const override { return true; }
    
    void set_session(uint32_t s) { session = s; }
    void set_signal_strength(float ss) { signal_strength = ss; }
    void set_noise_level(float nl) { noise_level = nl; }
    void set_snr(float s) { snr = s; }
    void set_grid_square(const std::string &gs) { grid_square = gs; }
};

class PropagationUpdate : public Message {
public:
    PropagationUpdate() {}
    
    std::string source_grid;
    std::string target_grid;
    float path_loss;
    QList<std::string> propagation_paths;
    
    QByteArray SerializeAsString() const override { return QByteArray(); }
    bool ParseFromString(const QByteArray &) override { return true; }
    void Clear() override {}
    bool IsInitialized() const override { return true; }
    
    void set_source_grid(const std::string &sg) { source_grid = sg; }
    void set_target_grid(const std::string &tg) { target_grid = tg; }
    void set_path_loss(float pl) { path_loss = pl; }
    void add_propagation_paths(const std::string &path) { propagation_paths.append(path); }
};
// Version message for protocol negotiation
class Version : public Message {
public:
    Version() : version(0), release(""), os(""), os_version("") {}
    virtual ~Version() {}
    
    uint32_t version;
    std::string release;
    std::string os;
    std::string os_version;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_version(uint32_t v) { version = v; }
    void set_release(const std::string &s) { release = s; }
    void set_os(const std::string &s) { os = s; }
    void set_os_version(const std::string &s) { os_version = s; }
};

// UDPTunnel for voice packet tunneling over TCP
class UDPTunnel : public Message {
public:
    UDPTunnel() {}
    virtual ~UDPTunnel() {}
    
    QByteArray packet;
    
    QByteArray SerializeAsString() const { return packet; }
    bool ParseFromString(const QByteArray &data) { packet = data; return true; }
    void Clear() { packet.clear(); }
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_packet(const QByteArray &p) { packet = p; }
};

// Authenticate message for client authentication
class Authenticate : public Message {
public:
    Authenticate() : strong_certificate(false) {}
    virtual ~Authenticate() {}
    
    std::string username;
    std::string password;
    QList<std::string> tokens;
    QList<std::string> celt_versions;
    bool strong_certificate;
    std::string opus;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_username(const std::string &s) { username = s; }
    void set_password(const std::string &s) { password = s; }
    void add_tokens(const std::string &s) { tokens.append(s); }
    void add_celt_versions(const std::string &s) { celt_versions.append(s); }
    void set_strong_certificate(bool b) { strong_certificate = b; }
    void set_opus(const std::string &s) { opus = s; }
};

// Ping message for keep-alive and RTT measurement
class Ping : public Message {
public:
    Ping() : timestamp(0), good(0), late(0), lost(0), resync(0) {}
    virtual ~Ping() {}
    
    uint64_t timestamp;
    uint32_t good;
    uint32_t late;
    uint32_t lost;
    uint32_t resync;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_timestamp(uint64_t t) { timestamp = t; }
    void set_good(uint32_t g) { good = g; }
    void set_late(uint32_t l) { late = l; }
    void set_lost(uint32_t l) { lost = l; }
    void set_resync(uint32_t r) { resync = r; }
};

// Reject message
class Reject : public Message {
public:
    Reject() : type(0) {}
    virtual ~Reject() {}
    
    uint32_t type;
    std::string reason;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_type(uint32_t t) { type = t; }
    void set_reason(const std::string &r) { reason = r; }
};

// ServerSync message
class ServerSync : public Message {
public:
    ServerSync() : session(0), max_bandwidth(0), permissions(0) {}
    virtual ~ServerSync() {}
    
    uint32_t session;
    uint32_t max_bandwidth;
    std::string welcome_text;
    uint32_t permissions;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_session(uint32_t s) { session = s; }
    void set_max_bandwidth(uint32_t b) { max_bandwidth = b; }
    void set_welcome_text(const std::string &t) { welcome_text = t; }
    void set_permissions(uint32_t p) { permissions = p; }
};

// ChannelRemove message
class ChannelRemove : public Message {
public:
    ChannelRemove() : channel_id(0) {}
    virtual ~ChannelRemove() {}
    
    uint32_t channel_id;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_channel_id(uint32_t c) { channel_id = c; }
};

// TextMessage for chat messages
class TextMessage : public Message {
public:
    TextMessage() {}
    virtual ~TextMessage() {}
    
    QList<uint32_t> session;
    QList<uint32_t> channel_id;
    QList<uint32_t> tree_id;
    std::string message;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void add_session(uint32_t s) { session.append(s); }
    void add_channel_id(uint32_t c) { channel_id.append(c); }
    void add_tree_id(uint32_t t) { tree_id.append(t); }
    void set_message(const std::string &m) { message = m; }
};

// UserRemove message
class UserRemove : public Message {
public:
    UserRemove() : session(0), actor(0), ban(false) {}
    virtual ~UserRemove() {}
    
    uint32_t session;
    uint32_t actor;
    std::string reason;
    bool ban;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_session(uint32_t s) { session = s; }
    void set_actor(uint32_t a) { actor = a; }
    void set_reason(const std::string &r) { reason = r; }
    void set_ban(bool b) { ban = b; }
};

// UserState message for user state changes
class UserState : public Message {
public:
    UserState() : session(0), actor(0), mute(false), deaf(false), suppress(false),
                  self_mute(false), self_deaf(false), priority_speaker(false),
                  recording(false), channel_id(0) {}
    virtual ~UserState() {}
    
    uint32_t session;
    uint32_t actor;
    std::string name;
    std::string comment;
    bool mute;
    bool deaf;
    bool suppress;
    bool self_mute;
    bool self_deaf;
    bool priority_speaker;
    bool recording;
    uint32_t channel_id;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_session(uint32_t s) { session = s; }
    void set_actor(uint32_t a) { actor = a; }
    void set_name(const std::string &n) { name = n; }
    void set_comment(const std::string &c) { comment = c; }
    void set_mute(bool b) { mute = b; }
    void set_deaf(bool b) { deaf = b; }
    void set_suppress(bool b) { suppress = b; }
    void set_self_mute(bool b) { self_mute = b; }
    void set_self_deaf(bool b) { self_deaf = b; }
    void set_priority_speaker(bool b) { priority_speaker = b; }
    void set_recording(bool b) { recording = b; }
    void set_channel_id(uint32_t c) { channel_id = c; }
};

// BanList message
class BanList : public Message {
public:
    BanList() : query(false) {}
    virtual ~BanList() {}
    
    bool query;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_query(bool q) { query = q; }
};

// ChannelState message for channel state changes
class ChannelState : public Message {
public:
    ChannelState() : channel_id(0), parent(0), temporary(false), position(0) {}
    virtual ~ChannelState() {}
    
    uint32_t channel_id;
    uint32_t parent;
    std::string name;
    QList<uint32_t> links;
    std::string description;
    bool temporary;
    int32_t position;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_parent(uint32_t p) { parent = p; }
    void set_name(const std::string &n) { name = n; }
    void add_links(uint32_t l) { links.append(l); }
    void set_description(const std::string &d) { description = d; }
    void set_temporary(bool t) { temporary = t; }
    void set_position(int32_t p) { position = p; }
};

// PermissionDenied message
class PermissionDenied : public Message {
public:
    PermissionDenied() : type(0), permission(0), channel_id(0), session(0) {}
    virtual ~PermissionDenied() {}
    
    uint32_t type;
    uint32_t permission;
    uint32_t channel_id;
    uint32_t session;
    std::string reason;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_type(uint32_t t) { type = t; }
    void set_permission(uint32_t p) { permission = p; }
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_session(uint32_t s) { session = s; }
    void set_reason(const std::string &r) { reason = r; }
};

// ACL message
class ACL : public Message {
public:
    ACL() : channel_id(0), inherit_acls(false) {}
    virtual ~ACL() {}
    
    uint32_t channel_id;
    bool inherit_acls;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_inherit_acls(bool i) { inherit_acls = i; }
};

// QueryUsers message
class QueryUsers : public Message {
public:
    QueryUsers() {}
    virtual ~QueryUsers() {}
    
    QList<uint32_t> ids;
    QList<std::string> names;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void add_ids(uint32_t id) { ids.append(id); }
    void add_names(const std::string &name) { names.append(name); }
};

// CryptSetup message
class CryptSetup : public Message {
public:
    CryptSetup() : client_nonce(""), server_nonce("") {}
    virtual ~CryptSetup() {}
    
    std::string key;
    std::string client_nonce;
    std::string server_nonce;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_key(const std::string &k) { key = k; }
    void set_client_nonce(const std::string &n) { client_nonce = n; }
    void set_server_nonce(const std::string &n) { server_nonce = n; }
};

// ContextActionModify message
class ContextActionModify : public Message {
public:
    ContextActionModify() : context(0), operation(0) {}
    virtual ~ContextActionModify() {}
    
    std::string action;
    std::string text;
    uint32_t context;
    uint32_t operation;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_action(const std::string &a) { action = a; }
    void set_text(const std::string &t) { text = t; }
    void set_context(uint32_t c) { context = c; }
    void set_operation(uint32_t o) { operation = o; }
};

// ContextAction message
class ContextAction : public Message {
public:
    ContextAction() : session(0), channel_id(0) {}
    virtual ~ContextAction() {}
    
    uint32_t session;
    uint32_t channel_id;
    std::string action;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_session(uint32_t s) { session = s; }
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_action(const std::string &a) { action = a; }
};

// UserList message
class UserList : public Message {
public:
    UserList() {}
    virtual ~UserList() {}
    
    QList<uint32_t> user_id;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void add_user_id(uint32_t id) { user_id.append(id); }
};

// VoiceTarget message
class VoiceTarget : public Message {
public:
    VoiceTarget() : id(0) {}
    virtual ~VoiceTarget() {}
    
    uint32_t id;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void*, int) { return true; }
    bool SerializeToArray(void*, int) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message&) {}
    void CopyFrom(const google::protobuf::Message&) {}
    void MergeFrom(const google::protobuf::Message&) {}
    
    void set_id(uint32_t i) { id = i; }
};

// PermissionQuery message for permission checks
class PermissionQuery : public Message {
public:
    PermissionQuery() : channel_id(0), permissions(0) {}
    virtual ~PermissionQuery() {}
    
    uint32_t channel_id;
    uint32_t permissions;
    QList<uint32_t> flush;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void set_channel_id(uint32_t c) { channel_id = c; }
    void set_permissions(uint32_t p) { permissions = p; }
    void add_flush(uint32_t f) { flush.append(f); }
};

// CodecVersion message
class CodecVersion : public Message {
public:
    CodecVersion() : alpha(0), beta(0), prefer_alpha(false), opus(false) {}
    virtual ~CodecVersion() {}
    
    int32_t alpha;
    int32_t beta;
    bool prefer_alpha;
    bool opus;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void set_alpha(int32_t a) { alpha = a; }
    void set_beta(int32_t b) { beta = b; }
    void set_prefer_alpha(bool p) { prefer_alpha = p; }
    void set_opus(bool o) { opus = o; }
};

// UserStats message
class UserStats : public Message {
public:
    UserStats() : session(0), stats_only(false) {}
    virtual ~UserStats() {}
    
    uint32_t session;
    bool stats_only;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void set_session(uint32_t s) { session = s; }
    void set_stats_only(bool so) { stats_only = so; }
};

// RequestBlob message
class RequestBlob : public Message {
public:
    RequestBlob() {}
    virtual ~RequestBlob() {}
    
    QList<uint32_t> session_texture;
    QList<uint32_t> session_comment;
    QList<uint32_t> channel_description;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void add_session_texture(uint32_t s) { session_texture.append(s); }
    void add_session_comment(uint32_t s) { session_comment.append(s); }
    void add_channel_description(uint32_t c) { channel_description.append(c); }
};

// ServerConfig message
class ServerConfig : public Message {
public:
    ServerConfig() : max_bandwidth(0), welcome_text(""), allow_html(false) {}
    virtual ~ServerConfig() {}
    
    uint32_t max_bandwidth;
    std::string welcome_text;
    bool allow_html;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void set_max_bandwidth(uint32_t b) { max_bandwidth = b; }
    void set_welcome_text(const std::string &t) { welcome_text = t; }
    void set_allow_html(bool h) { allow_html = h; }
};

// SuggestConfig message
class SuggestConfig : public Message {
public:
    SuggestConfig() : version(0), positional(false), push_to_talk(false) {}
    virtual ~SuggestConfig() {}
    
    uint32_t version;
    bool positional;
    bool push_to_talk;
    
    QByteArray SerializeAsString() const { return QByteArray(); }
    bool ParseFromString(const QByteArray &) { return true; }
    void Clear() {}
    bool IsInitialized() const { return true; }
    
    // Required overrides from google::protobuf::Message
    bool ParseFromArray(const void* data, int size) { return true; }
    bool SerializeToArray(void* data, int size) const { return true; }
    int ByteSize() const { return 0; }
    void CheckTypeAndMergeFrom(const google::protobuf::Message& other) {}
    void CopyFrom(const google::protobuf::Message& from) {}
    void MergeFrom(const google::protobuf::Message& from) {}
    
    void set_version(uint32_t v) { version = v; }
    void set_positional(bool p) { positional = p; }
    void set_push_to_talk(bool p) { push_to_talk = p; }
};

// Additional message types would be defined here...

} // namespace MumbleProto

#endif // MUMBLE_MURMUR_MUMBLE_PB_H_