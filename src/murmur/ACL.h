// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ACL_H_
#define MUMBLE_MURMUR_ACL_H_

#include <QFlags>
#include <QHash>
#include <QSet>
#include <QString>

class Server;
class Channel;
class User;
class ServerUser;

// Channel Access Control List
class ChanACL {
public:
    // Permission flags
    enum Permission {
        None         = 0x00000000,
        Write        = 0x00000001,
        Traverse     = 0x00000002,
        Enter        = 0x00000004,
        Speak        = 0x00000008,
        MuteDeafen   = 0x00000010,
        Move         = 0x00000020,
        MakeChannel  = 0x00000040,
        LinkChannel  = 0x00000080,
        Whisper      = 0x00000100,
        TextMessage  = 0x00000200,
        MakeTempChannel = 0x00000400,
        Listen       = 0x00000800,
        
        // SuperMorse custom permissions
        SetGridLocator = 0x10000000,
        ModifyBandplan = 0x20000000,
        ConfigurePropagation = 0x40000000,
        
        All          = 0xFFFFFFFF
    };
    Q_DECLARE_FLAGS(Permissions, Permission)
    
    typedef QFlags<Permission> Perm;
    
    // Basic structures for ACL cache
    struct ACLCache {
        QHash<Channel *, Permissions> channelPermissions;
        QHash<QPair<Channel *, int>, Permissions> groupPermissions;
        QSet<Channel *> requestedChannels;
        
        void clear() {
            channelPermissions.clear();
            groupPermissions.clear();
            requestedChannels.clear();
        }
    };
    
    ChanACL(Channel *chan = nullptr);
    ~ChanACL();
    
    // Member variables
    int iId;
    bool bApplyHere;
    bool bApplySubs;
    bool bInherited;
    
    // User and group
    int iUserId;
    QString qsGroup;
    
    // Permissions
    QFlags<Permission> pAllow;
    QFlags<Permission> pDeny;
    
    // Channel
    Channel *c;
    
    // Gets all user groups (static method)
    static QSet<QString> getRegisteredUserGroups(Server *server);
    
    // Helper methods
    static bool hasPermission(ServerUser *user, Channel *channel, QFlags<Permission> permission);
    static QFlags<Permission> effectivePermissions(ServerUser *user, Channel *channel);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ChanACL::Permissions)

#endif // MUMBLE_MURMUR_ACL_H_