#include "run_guard.hpp"
#include <QCryptographicHash>

static inline auto generateKeyHash(const QString& key, const QString& salt) noexcept -> QString
{
        QByteArray data;

        data.append(key.toUtf8());
        data.append(salt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
}

RunGuard::RunGuard(const QString& key) noexcept
        : key(key)
        , memLockKey(generateKeyHash(key, "_memLockKey"))
        , sharedmemKey(generateKeyHash(key, "_sharedmemKey"))
        , sharedMem(sharedmemKey)
        , memLock(memLockKey, 1)
{
        memLock.acquire();
        {
                QSharedMemory fix(sharedmemKey);
                fix.attach();
        }
        memLock.release();
}

RunGuard::~RunGuard() noexcept
{
        release();
}

auto RunGuard::isAnotherRunning() noexcept -> bool
{
        if (sharedMem.isAttached())
                return false;

        memLock.acquire();
        const bool isRunning = sharedMem.attach();
        if (isRunning)
                sharedMem.detach();
        memLock.release();

        return isRunning;
}

auto RunGuard::tryToRun() noexcept -> bool
{
        if (isAnotherRunning())   // Extra check
                return false;

        memLock.acquire();
        const bool result = sharedMem.create(sizeof(quint64));
        memLock.release();
        if (!result)
        {
                release();
                return false;
        }

        return true;
}

auto RunGuard::release() noexcept -> void
{
        memLock.acquire();
        if (sharedMem.isAttached())
                sharedMem.detach();
        memLock.release();
}
