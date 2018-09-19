/* SCE CONFIDENTIAL
 * PlayStation(R)4 Programmer Tool Runtime Library Release 03.508.031
 * Copyright (C) 2013 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

#define LIBRARY_IMPL  (1)
#include "stdafx.h"
#include "thread_util.h"
#include "sampleutil.h"

common::Util::Mutex::Mutex(const char *name)
{
	ScePthreadMutexattr mutexAttr;
	scePthreadMutexattrInit(&mutexAttr);
	int ret = scePthreadMutexattrSettype(&mutexAttr, SCE_PTHREAD_MUTEX_RECURSIVE);
	ret = scePthreadMutexInit(&m_mutex, &mutexAttr, name);	
	scePthreadMutexattrDestroy(&mutexAttr);
}

int common::Util::Mutex::lock(void)
{
	return scePthreadMutexLock(&m_mutex);
}

int common::Util::Mutex::unlock(void)
{
	return scePthreadMutexUnlock(&m_mutex);
}

common::Util::Mutex::~Mutex(void)
{
	scePthreadMutexDestroy(&m_mutex);
}

int common::Util::Semaphore::initialize(const char* name, int initCount, int maxCount)
{
	return sceKernelCreateSema(&m_semaphore, name, SCE_KERNEL_SEMA_ATTR_TH_FIFO, initCount, maxCount, NULL);
}

int common::Util::Semaphore::finalize(void)
{
	return sceKernelDeleteSema(m_semaphore);
}

int common::Util::Semaphore::signal(int signalCount)
{
	return sceKernelSignalSema(m_semaphore, 1);
}

int common::Util::Semaphore::wait(int needCount)
{
	return sceKernelWaitSema(m_semaphore, 1, NULL);
}

int common::Util::Semaphore::poll(int needCount)
{
	return sceKernelPollSema(m_semaphore, 1);
}

common::Util::FocusLock::FocusLock(common::Util::Mutex *mutex)
{
	m_mutex = mutex;
	int ret = m_mutex->lock();
	(void)ret;
}

common::Util::FocusLock::~FocusLock(void)
{
	int ret = m_mutex->unlock();
	(void)ret;
}

common::Util::EventFlag::EventFlag(const char *name)
{
	int ret = sceKernelCreateEventFlag(&m_eventflag,
									   name != NULL ? name : "",
									   SCE_KERNEL_EVF_ATTR_TH_FIFO | SCE_KERNEL_EVF_ATTR_MULTI,
									   0x0,
									   NULL);
	(void)ret;
}

common::Util::EventFlag::~EventFlag(void)
{
	int ret = sceKernelDeleteEventFlag(m_eventflag);
	(void)ret;
}

void common::Util::EventFlag::set(void)
{
	int ret = sceKernelSetEventFlag(m_eventflag, 0x1);
	(void)ret;
}

void common::Util::EventFlag::wait(void)
{
	int ret = sceKernelWaitEventFlag(m_eventflag,
									 0x1,
									 SCE_KERNEL_EVF_WAITMODE_OR | SCE_KERNEL_EVF_WAITMODE_CLEAR_ALL,
									 NULL,
									 NULL);
	(void)ret;
}

// ------------------------------------------------------------------------------------------

void* common::Util::Thread::fakeEntry(void *arg)
{
	Thread* self = (Thread*)arg;
	void *ret = self->m_threadEntry(self->m_threadArg);
	self->m_isFinished = true;
	return ret;
}

common::Util::Thread::Thread(void)
{
	m_threadEntry = NULL;
	m_threadArg = NULL;
	m_isFinished = false;
}
int common::Util::Thread::start(const char *name, Entry threadEntry, void* arg)
{
	int ret;
	
	ScePthreadAttr attr;
	ret = scePthreadAttrInit(&attr);
	if(ret < 0){
		return ret;
	}
	ret = scePthreadAttrSetdetachstate(&attr, SCE_PTHREAD_CREATE_JOINABLE);
	if(ret < 0){
		scePthreadAttrDestroy(&attr);
		return ret;
	}
	ret = scePthreadAttrSetstacksize(&attr, kStackSize);
	if(ret < 0){
		scePthreadAttrDestroy(&attr);
		return ret;
	}
	
	m_threadEntry = threadEntry;
	m_threadArg = arg;
	m_isFinished = false;
	ret = scePthreadCreate(&m_thread, &attr, fakeEntry, this, name);
	scePthreadAttrDestroy(&attr);
	if (ret < 0) {
		return ret;
	}
	return SCE_OK;
}

int common::Util::Thread::join(void **outRet)
{
	return scePthreadJoin(m_thread, outRet);
}

int common::Util::Thread::tryJoin(void **outRet)
{
	if(!m_isFinished){
		return -1;
	}
	return scePthreadJoin(m_thread, outRet);
}

int common::Util::DirectMemoryHeap::Heap::initialize(size_t alignment, size_t size, int memoryType, size_t maxPageSize)
{
	int ret;
	(void)ret;
	m_size = size;
	ret = sceKernelAllocateDirectMemory(
		/*searchStart*/ 0,
		/*searchEnd*/   SCE_KERNEL_MAIN_DMEM_SIZE,
		/*len*/         size,
		/*alignment*/   alignment,
		/*memoryType*/  memoryType,
		/*offsetOut*/   &m_offset);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = sceKernelMapDirectMemory(&m_basePointer,
		size,
		SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE | SCE_KERNEL_PROT_GPU_ALL,
		0, //flags
		m_offset,
		maxPageSize);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	m_mspace = sceLibcMspaceCreate("Heap", m_basePointer, size, 0);
	//SCE_SAMPLE_UTIL_ASSERT(m_mspace != NULL);

	return SCE_OK;
}


int common::Util::DirectMemoryHeap::Heap::finalize(void)
{
	if (m_mspace != NULL) {
		sceLibcMspaceDestroy(m_mspace);
		m_mspace = NULL;
	}

	int ret;
	ret = sceKernelMunmap(m_basePointer, m_size);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = sceKernelReleaseDirectMemory(m_offset, m_size);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	return SCE_OK;
}


void *common::Util::DirectMemoryHeap::Heap::allocate(size_t size)
{
	return sceLibcMspaceMalloc(m_mspace, size);
}
void *common::Util::DirectMemoryHeap::Heap::memalign(size_t boundary, size_t size)
{
	return sceLibcMspaceMemalign(m_mspace, boundary, size);
}
void common::Util::DirectMemoryHeap::Heap::free(void* ptr)
{
	sceLibcMspaceFree(m_mspace, ptr);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
}




int common::Util::DirectMemoryHeap::initialize(size_t onionSize, size_t garlicSize)
{
	int ret;
	ret = garlic.initialize(2 * 1024 * 1024, garlicSize, SCE_KERNEL_WC_GARLIC, 2 * 1024 * 1024);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = onion.initialize(2 * 1024 * 1024, onionSize, SCE_KERNEL_WB_ONION, 2 * 1024 * 1024);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	return SCE_OK;
}

int common::Util::DirectMemoryHeap::finalize(void)
{
	int ret;
	ret = garlic.finalize();
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = onion.finalize();
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	return SCE_OK;
}
//-----------------------------------------------------------------------------