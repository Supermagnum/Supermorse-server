// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_TIMER_H_
#define MUMBLE_MURMUR_TIMER_H_

#include <QDateTime>
#include <QElapsedTimer>
#include <QMutex>
#include <QString>
#include <QObject>

/**
 * @brief The Timer class provides high-resolution timing functionality.
 * 
 * This class is used for accurate time measurement, including server uptime,
 * connection timing, and performance monitoring.
 */
class Timer {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a new timer in stopped state.
     */
    Timer();
    
    /**
     * @brief Constructor that automatically starts the timer
     * 
     * @param autoStart If true, automatically start the timer
     */
    Timer(bool autoStart);
    
    /**
     * @brief Destructor
     */
    ~Timer();
    
    /**
     * @brief Start the timer
     * 
     * If the timer is already running, it will be restarted.
     */
    void start();
    
    /**
     * @brief Stop the timer
     * 
     * This stops the timer but keeps the elapsed time.
     */
    void stop();
    
    /**
     * @brief Restart the timer
     * 
     * This resets the elapsed time and starts the timer again.
     */
    void restart();
    
    /**
     * @brief Check if the timer is active
     * 
     * @return True if the timer is running
     */
    bool isActive() const;
    
    /**
     * @brief Get the elapsed time in milliseconds
     * 
     * @return Elapsed time in milliseconds
     */
    qint64 elapsed() const;
    
    /**
     * @brief Get the elapsed time in seconds
     * 
     * @return Elapsed time in seconds
     */
    double elapsedSeconds() const;
    
    /**
     * @brief Get a formatted string of the elapsed time
     * 
     * @return Formatted string with the elapsed time
     */
    QString elapsedString() const;
    
    /**
     * @brief Get the time when the timer was started
     * 
     * @return QDateTime representing the start time
     */
    QDateTime startTime() const;
    
    /**
     * @brief Add time to the timer
     * 
     * @param msecs Time to add in milliseconds
     */
    void addTime(qint64 msecs);
    
    /**
     * @brief Check if a timeout has occurred
     * 
     * @param timeout Timeout value in milliseconds
     * @return True if the elapsed time exceeds the timeout value
     */
    bool hasExpired(qint64 timeout) const;
    
private:
    QElapsedTimer m_timer;
    QDateTime m_startTime;
    qint64 m_additionalTime;
    bool m_active;
    mutable QMutex m_mutex;
};

/**
 * @brief The TimerManager class manages multiple timers.
 * 
 * This class provides a centralized way to manage multiple timers
 * and is useful for tracking different timers in the server.
 */
class TimerManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * 
     * @param parent Parent QObject
     */
    TimerManager(QObject *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~TimerManager();
    
    /**
     * @brief Create a new timer with the given name
     * 
     * @param name Name of the timer
     * @param autoStart If true, automatically start the timer
     * @return Timer* Pointer to the created timer
     */
    Timer* createTimer(const QString &name, bool autoStart = true);
    
    /**
     * @brief Get a timer by name
     * 
     * @param name Name of the timer to retrieve
     * @return Timer* Pointer to the timer, or nullptr if not found
     */
    Timer* getTimer(const QString &name);
    
    /**
     * @brief Remove a timer by name
     * 
     * @param name Name of the timer to remove
     */
    void removeTimer(const QString &name);
    
    /**
     * @brief Clear all timers
     */
    void clear();
    
    /**
     * @brief Get the server uptime timer
     * 
     * @return Timer* Pointer to the server uptime timer
     */
    Timer* getUptimeTimer();
    
private:
    QHash<QString, Timer*> m_timers;
    Timer *m_uptimeTimer;
    QMutex m_mutex;
};

#endif // MUMBLE_MURMUR_TIMER_H_