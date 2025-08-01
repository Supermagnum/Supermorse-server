// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_HOSTADDRESS_H_
#define MUMBLE_MURMUR_HOSTADDRESS_H_

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtNetwork/QHostAddress>

/**
 * @brief The HostAddress class extends QHostAddress with additional networking functionality
 * 
 * This class provides enhanced IP address handling, particularly for ban management
 * and subnet operations in the Mumble server.
 */
class HostAddress : public QHostAddress {
public:
    /** 
     * @brief Default constructor
     */
    HostAddress() : QHostAddress() {}

    /**
     * @brief Constructor from QHostAddress
     * @param address QHostAddress to copy
     */
    HostAddress(const QHostAddress &address) : QHostAddress(address) {}

    /**
     * @brief Constructor from IPv4 address
     * @param ip4 IPv4 address
     */
    HostAddress(quint32 ip4) : QHostAddress(ip4) {}

    /**
     * @brief Constructor from IPv6 address
     * @param ip6 IPv6 address
     */
    HostAddress(const Q_IPV6ADDR &ip6) : QHostAddress(ip6) {}

    /**
     * @brief Constructor from string representation
     * @param address String representation of address
     */
    HostAddress(const QString &address) : QHostAddress(address) {}

    /**
     * @brief Constructor from byte array
     * @param address Byte array representation of address
     */
    HostAddress(const QByteArray &address) : QHostAddress(QString::fromUtf8(address)) {}

    /**
     * @brief Get the host as a lower-case string
     * @return Lower-case string representation of address
     */
    QString toLower() const {
        return toString().toLower();
    }

    /**
     * @brief Check if this address is in a subnet
     * @param subnet Subnet to check
     * @param bits Number of bits in subnet mask
     * @return True if address is in the subnet
     */
    bool isInSubnet(const HostAddress &subnet, int bits) const;

    /**
     * @brief Convert to a binary representation
     * @return Binary representation of the address
     */
    QByteArray toByteArray() const;

    /**
     * @brief Match with another address considering subnet bits
     * @param other Other address to match against
     * @param bits Number of bits to match
     * @return True if the addresses match within the specified bits
     */
    bool match(const HostAddress &other, int bits) const;
};

/**
 * @brief Hash function for HostAddress
 * @param ha Host address to hash
 * @return Hash value
 */
uint qHash(const HostAddress &ha);

#endif // MUMBLE_MURMUR_HOSTADDRESS_H_