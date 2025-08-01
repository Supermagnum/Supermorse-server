// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QDateTime>
#include <QElapsedTimer>
#include <QMutex>
#include <QString>
#include <QObject>

#include "Timer.h"

Timer::Timer() : m_additionalTime(0), m_active(false) {
}

Timer::Timer(bool autoStart) : m_additionalTime(0), m_active(false) {
    if (autoStart) {
        start();
    }
}

Timer::~Timer() {
}

void Timer::start() {
    QMutexLocker locker(&m_mutex);
    m_startTime = QDateTime::currentDateTime();
    m_timer.start();
    m_active = true;
}

void Timer::stop() {
    QMutexLocker locker(&m_mutex);
    if (m_active) {
        m_additionalTime += m_timer.elapsed();
        m_active = false;
    }
}

void Timer::restart() {
    QMutexLocker locker(&m_mutex);
    m_additionalTime = 0;
    m_startTime = QDateTime::currentDateTime();
    m_timer.restart();
    m_active = true;
}

bool Timer::isActive() const {
    QMutexLocker locker(&m_mutex);
    return m_active;
}

qint64 Timer::elapsed() const {
    QMutexLocker locker(&m_mutex);
    if (m_active) {
        return m_additionalTime + m_timer.elapsed();
    } else {
        return m_additionalTime;
    }
}

double Timer::elapsedSeconds() const {
    return elapsed() / 1000.0;
}

QString Timer::elapsedString() const {
    qint64 e = elapsed();
    int hours = e / (1000 * 60 * 60);
    int minutes = (e / (1000 * 60)) % 60;
    int seconds = (e / 1000) % 60;
    int milliseconds = e % 1000;
    
    return QString("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(milliseconds, 3, 10, QChar('0'));
}

QDateTime Timer::startTime() const {
    QMutexLocker locker(&m_mutex);
    return m_startTime;
}

void Timer::addTime(qint64 msecs) {
    QMutexLocker locker(&m_mutex);
    m_additionalTime += msecs;
}

bool Timer::hasExpired(qint64 timeout) const {
    return elapsed() >= timeout;
}

// TimerManager implementation
TimerManager::TimerManager(QObject *parent) : QObject(parent), m_uptimeTimer(nullptr) {
    m_uptimeTimer = new Timer(true);
}

TimerManager::~TimerManager() {
    clear();
    delete m_uptimeTimer;
}

Timer* TimerManager::createTimer(const QString &name, bool autoStart) {
    QMutexLocker locker(&m_mutex);
    Timer *timer = new Timer(autoStart);
    m_timers.insert(name, timer);
    return timer;
}

Timer* TimerManager::getTimer(const QString &name) {
    QMutexLocker locker(&m_mutex);
    return m_timers.value(name, nullptr);
}

void TimerManager::removeTimer(const QString &name) {
    QMutexLocker locker(&m_mutex);
    Timer *timer = m_timers.take(name);
    if (timer) {
        delete timer;
    }
}

void TimerManager::clear() {
    QMutexLocker locker(&m_mutex);
    qDeleteAll(m_timers);
    m_timers.clear();
}

Timer* TimerManager::getUptimeTimer() {
    return m_uptimeTimer;
}