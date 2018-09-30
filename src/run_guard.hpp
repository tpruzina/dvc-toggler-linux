#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard
{

public:
        RunGuard(const QString &key) noexcept;
        ~RunGuard() noexcept;

        bool isAnotherRunning() noexcept;
        bool tryToRun() noexcept;
        void release() noexcept;

private:
        const QString key;
        const QString memLockKey;
        const QString sharedmemKey;

        QSharedMemory sharedMem;
        QSystemSemaphore memLock;

        Q_DISABLE_COPY(RunGuard)
};
#endif // RUNGUARD_HPP
