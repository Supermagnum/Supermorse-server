// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_MUMBLEMESSAGES_H_
#define MUMBLE_MURMUR_MUMBLEMESSAGES_H_

/**
 * @brief Macro that expands to all Mumble TCP message types
 * 
 * This macro is used to define operations on all message types
 * at once by expanding the provided macro name for each message type.
 */
#define MUMBLE_ALL_TCP_MESSAGES \
    PROCESS_MUMBLE_TCP_MESSAGE(Version, 0) \
    PROCESS_MUMBLE_TCP_MESSAGE(UDPTunnel, 1) \
    PROCESS_MUMBLE_TCP_MESSAGE(Authenticate, 2) \
    PROCESS_MUMBLE_TCP_MESSAGE(Ping, 3) \
    PROCESS_MUMBLE_TCP_MESSAGE(Reject, 4) \
    PROCESS_MUMBLE_TCP_MESSAGE(ServerSync, 5) \
    PROCESS_MUMBLE_TCP_MESSAGE(ChannelRemove, 6) \
    PROCESS_MUMBLE_TCP_MESSAGE(ChannelState, 7) \
    PROCESS_MUMBLE_TCP_MESSAGE(UserRemove, 8) \
    PROCESS_MUMBLE_TCP_MESSAGE(UserState, 9) \
    PROCESS_MUMBLE_TCP_MESSAGE(BanList, 10) \
    PROCESS_MUMBLE_TCP_MESSAGE(TextMessage, 11) \
    PROCESS_MUMBLE_TCP_MESSAGE(PermissionDenied, 12) \
    PROCESS_MUMBLE_TCP_MESSAGE(ACL, 13) \
    PROCESS_MUMBLE_TCP_MESSAGE(QueryUsers, 14) \
    PROCESS_MUMBLE_TCP_MESSAGE(CryptSetup, 15) \
    PROCESS_MUMBLE_TCP_MESSAGE(ContextActionModify, 16) \
    PROCESS_MUMBLE_TCP_MESSAGE(ContextAction, 17) \
    PROCESS_MUMBLE_TCP_MESSAGE(UserList, 18) \
    PROCESS_MUMBLE_TCP_MESSAGE(VoiceTarget, 19) \
    PROCESS_MUMBLE_TCP_MESSAGE(PermissionQuery, 20) \
    PROCESS_MUMBLE_TCP_MESSAGE(CodecVersion, 21) \
    PROCESS_MUMBLE_TCP_MESSAGE(UserStats, 22) \
    PROCESS_MUMBLE_TCP_MESSAGE(RequestBlob, 23) \
    PROCESS_MUMBLE_TCP_MESSAGE(ServerConfig, 24) \
    PROCESS_MUMBLE_TCP_MESSAGE(SuggestConfig, 25) \
    PROCESS_MUMBLE_TCP_MESSAGE(PluginDataTransmission, 26) \
    PROCESS_MUMBLE_TCP_MESSAGE(ChannelListener, 27) \
    PROCESS_MUMBLE_TCP_MESSAGE(HFBandSimulationUpdate, 28) \
    PROCESS_MUMBLE_TCP_MESSAGE(SignalQualityUpdate, 29) \
    PROCESS_MUMBLE_TCP_MESSAGE(PropagationUpdate, 30)

#endif // MUMBLE_MURMUR_MUMBLEMESSAGES_H_