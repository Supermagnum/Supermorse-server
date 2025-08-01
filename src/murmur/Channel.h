// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_CHANNEL_H_
#define MUMBLE_MURMUR_CHANNEL_H_

#include <QtCore/QString>
#include <QtCore/QSet>

class ServerUser;

class Channel {
public:
    int iId;           // Channel ID
    QString qsName;    // Channel name
    QString qsDesc;    // Channel description
    QSet<int> qsPermLinks;  // Links to other channels

    // Constructor
    Channel(int id, const QString &name) : iId(id), qsName(name), qsDesc("") {
    }

    // Add additional methods if needed for compatibility
};

#endif // MUMBLE_MURMUR_CHANNEL_H_