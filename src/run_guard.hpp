#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard
{

public:
	RunGuard(const QString& key);
	~RunGuard();

	bool isAnotherRunning();
	bool tryToRun();
	void release();

private:
	const QString key;
	const QString memLockKey;
	const QString sharedmemKey;

	QSharedMemory sharedMem;
	QSystemSemaphore memLock;

	Q_DISABLE_COPY(RunGuard)
};
#endif // RUNGUARD_HPP