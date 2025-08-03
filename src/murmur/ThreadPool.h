// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_THREADPOOL_H_
#define MUMBLE_MURMUR_THREADPOOL_H_

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QDebug>

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <vector>
#include <memory>
#include <atomic>

/**
 * @brief The ThreadPool class provides a pool of worker threads for parallel task execution.
 * 
 * This class manages a group of worker threads and distributes tasks among them to enable
 * parallel execution across multiple CPU cores. It is designed to improve the performance
 * of computationally intensive operations by utilizing all available processor cores.
 */
class ThreadPool : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for ThreadPool.
     * 
     * Creates a thread pool with a specified number of worker threads.
     * 
     * @param numThreads The number of worker threads to create. If 0, it will use the number of CPU cores.
     * @param parent The parent QObject
     */
    explicit ThreadPool(int numThreads = 0, QObject *parent = nullptr);
    
    /**
     * @brief Destructor for ThreadPool.
     * 
     * Ensures all worker threads are properly terminated.
     */
    virtual ~ThreadPool();
    
    /**
     * @brief Enqueues a task to be executed by a worker thread.
     * 
     * @tparam F The function type
     * @tparam Args The argument types
     * @param func The function to execute
     * @param args The arguments to pass to the function
     * @return A future containing the result of the function
     */
    template<class F, class... Args>
    auto enqueue(F&& func, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    /**
     * @brief Gets the number of worker threads in the pool.
     * 
     * @return The number of worker threads
     */
    int threadCount() const;
    
    /**
     * @brief Gets the number of tasks currently in the queue.
     * 
     * @return The number of queued tasks
     */
    int queuedTaskCount() const;
    
    /**
     * @brief Waits for all tasks to complete.
     */
    void waitForDone();
    
    /**
     * @brief Gets the optimal number of threads based on CPU cores.
     * 
     * @return The recommended number of threads
     */
    static int optimalThreadCount();

private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    
    mutable std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stop;
    std::atomic<int> m_activeThreads;
    
    int m_threadCount;
    
    void workerThread();
};

// Template implementation must be in the header file
template<class F, class... Args>
auto ThreadPool::enqueue(F&& func, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    
    using return_type = typename std::result_of<F(Args...)>::type;
    
    // Create a packaged task with the function and its arguments
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(func), std::forward<Args>(args)...)
    );
    
    // Get the future result
    std::future<return_type> result = task->get_future();
    
    // Add the task to the queue
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        
        // Don't allow enqueueing after stopping the pool
        if (m_stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        
        // Wrap the packaged task in a void function
        m_tasks.emplace([task]() { (*task)(); });
    }
    
    // Notify one worker thread that there's a new task
    m_condition.notify_one();
    
    return result;
}

#endif // MUMBLE_MURMUR_THREADPOOL_H_