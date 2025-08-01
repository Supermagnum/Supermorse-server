// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "database/MariaDBConnectionParameter.h"
#include "../../database/ConnectionParameter.h"

namespace mumble {
namespace db {

/**
 * @brief Implementation of actual ConnectionParameter that will be returned by toConnectionParameter()
 */
class MariaDBConnectionParameterImpl : public ConnectionParameter {
public:
    MariaDBConnectionParameterImpl(const MariaDBConnectionParameter& param)
        : m_param(param) {
    }
    
    virtual ~MariaDBConnectionParameterImpl() = default;
    
    QString driverName() const override {
        return m_param.driverName();
    }
    
    QString databaseName() const override {
        return m_param.databaseName();
    }
    
    QMap<QString, QVariant> options() const override {
        return m_param.options();
    }
    
    QString hostName() const override {
        return m_param.hostName();
    }
    
    int port() const override {
        return m_param.port();
    }
    
    QString userName() const override {
        return m_param.userName();
    }
    
    QString password() const override {
        return m_param.password();
    }
    
    bool isValid() const override {
        return m_param.isValid();
    }
    
    ConnectionParameter* clone() const override {
        return new MariaDBConnectionParameterImpl(m_param);
    }
    
private:
    MariaDBConnectionParameter m_param;
};

ConnectionParameter* MariaDBConnectionParameter::toConnectionParameter() const {
    return new MariaDBConnectionParameterImpl(*this);
}

} // namespace db
} // namespace mumble