#pragma once

#include <QObject>

#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "QtCallback.h"

class Task {
public:
    virtual void run() = 0;

    QtCallback& callback() {
        return callback_;
    }
    virtual ~Task() {
    }
private:
    QtCallback callback_;
};
typedef std::shared_ptr< Task > TaskPtr;
Q_DECLARE_METATYPE( std::shared_ptr< Task > )

class TaskProcessor : public QObject {
    Q_OBJECT
public:
    TaskProcessor();
    ~TaskProcessor();

    void schedule( std::shared_ptr< Task > task );
    void remove( std::shared_ptr< Task > task );
    void stop();
    void start();
    void clear();

    bool isRunning() const;
    bool isWorking() const;
    size_t queueSize() const;

    const std::list< std::shared_ptr< Task > >& getScheduledTasks() const;
    const std::shared_ptr< Task >& getCurrentTask() const;

    bool isScheduled( std::shared_ptr< Task > task ) const;
    bool isRunning( std::shared_ptr< Task > task ) const;
signals:
    void scheduledTask( std::shared_ptr< Task > );
    void removedTask( std::shared_ptr< Task > );
    void startingTask( std::shared_ptr< Task > );
    void finishedTask( std::shared_ptr< Task > );

private:
    TaskProcessor( const TaskProcessor& ) = delete;
    const TaskProcessor& operator=( const TaskProcessor& );

    std::list< std::shared_ptr< Task > > tasks_;
    std::shared_ptr< Task > currentTask_;
    mutable std::mutex mutex_;
    std::unique_ptr< std::thread > thread_;
    std::condition_variable blocker_;
    bool goOn_;

    void threadLoop();
};
