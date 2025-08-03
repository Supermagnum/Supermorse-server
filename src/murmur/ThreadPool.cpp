// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ThreadPool.h"

#include <QtCore/QThread>
#include <QtCore/QtGlobal>

ThreadPool::ThreadPool(int numThreads, QObject *parent)
    : QObject(parent)
    , m_stop(false)
    , m_activeThreads(0) {
    
    // Determine the number of worker threads to create
    if (numThreads <= 0) {
        m_threadCount = optimalThreadCount();
    } else {
        m_threadCount = numThreads;
    }
    
    qDebug() << "Creating ThreadPool with" << m_threadCount << "worker threads";
    
    // Create and start the worker threads
    m_workers.reserve(m_threadCount);
    for (int i = 0; i < m_threadCount; ++i) {
        m_workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    // Signal worker threads to stop
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
    
    // Notify all waiting threads
    m_condition.notify_all();
    
    // Wait for all threads to finish
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    
    qDebug() << "ThreadPool destroyed";
}

int ThreadPool::threadCount() const {
    return m_threadCount;
}

int ThreadPool::queuedTaskCount() const {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return static_cast<int>(m_tasks.size());
}

void ThreadPool::waitForDone() {
    // We'll wait until there are no active threads and no queued tasks
    while (true) {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        if (m_tasks.empty() && m_activeThreads == 0) {
            break;
        }
        
        // Wait for a short time and check again
        lock.unlock();
        QThread::msleep(10);
    }
    
    qDebug() << "All tasks completed";
}

int ThreadPool::optimalThreadCount() {
    // Get the number of hardware threads
    int cores = std::thread::hardware_concurrency();
    
    // If we can't detect, use a reasonable default
    if (cores <= 0) {
        cores = 4;
    }
    
    qDebug() << "Detected" << cores << "CPU cores";
    
    // Use all available cores
    return cores;
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        
        // Wait for a task or stop signal
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            
            // Wait until there's a task or we're stopping
            m_condition.wait(lock, [this] {
                return m_stop || !m_tasks.empty();
            });
            
            // Exit if we're stopping and there are no more tasks
            if (m_stop && m_tasks.empty()) {
                return;
            }
            
            // Get the next task
            task = std::move(m_tasks.front());
            m_tasks.pop();
            
            // Increment active thread count
            ++m_activeThreads;
        }
        
        // Execute the task
        try {
            task();
        } catch (const std::exception& e) {
            qWarning() << "Exception in ThreadPool task:" << e.what();
        } catch (...) {
            qWarning() << "Unknown exception in ThreadPool task";
        }
        
        // Decrement active thread count
        --m_activeThreads;
    }
}