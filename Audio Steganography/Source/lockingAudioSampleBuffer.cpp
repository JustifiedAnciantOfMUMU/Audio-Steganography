#include "lockingAudioSampleBuffer.h"

void LockingAudioSampleBuffer::Lock()
{
	//DBG("Locking");
	m_loadLock.enter();
	//DBG("Locked");
}

void LockingAudioSampleBuffer::Unlock()
{
	//DBG("Unlocking");
	m_loadLock.exit();
	//DBG("Unlocked");
}
