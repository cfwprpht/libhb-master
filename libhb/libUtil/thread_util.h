/* SCE CONFIDENTIAL
 * PlayStation(R)4 Programmer Tool Runtime Library Release 03.508.031
 * Copyright (C) 2013 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

#pragma once

#include <kernel.h>
#include <semaphore.h>
#include <mspace.h>

#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

//#define SCE_SAMPLE_UTIL_ASSERT_EQUAL(value, expected)	sce::SampleUtil::Internal::AssertEqual32(__FILE__, __LINE__, value, expected, "Assertion failed, values not equal. actual value=%#x, expected value=%#x\n", value, expected)

namespace common {
    namespace Util {
		class PRX_INTERFACE Mutex {
		private:
			ScePthreadMutex m_mutex;
		public:
			Mutex(const char* name=NULL);
			int lock(void);
			int unlock(void);
			~Mutex(void);
		};

		class PRX_INTERFACE Semaphore {
		private:
			SceKernelSema m_semaphore;
		public:
			int initialize(const char* name, int initCount, int maxCount);
			int finalize(void);
			int signal(int signalCount);
			int wait(int needCount);
			int poll(int needCount);
		};

		class PRX_INTERFACE FocusLock {
		private:
			Mutex *m_mutex;
		public:
			FocusLock(Mutex *mutex);
			~FocusLock(void);
		};

		class PRX_INTERFACE EventFlag {
		private:
			SceKernelEventFlag m_eventflag;
		public:
			EventFlag(const char* name=NULL);
			~EventFlag(void);
			void set(void);
			void wait(void);
		};

		class PRX_INTERFACE Thread {
		public:
			typedef void* (*Entry)(void *arg);
			static const uint32_t kStackSize = 16 * 1024 * 1024;
			
			Thread(void);
			int start(const char *name, Entry threadEntry, void* arg);
			int join(void **outRet);
			int tryJoin(void **outRet);
			
		private:
			ScePthread m_thread;
			Entry m_threadEntry;
			void *m_threadArg;
			bool m_isFinished;
			static void* fakeEntry(void *arg);
		};

		class PRX_INTERFACE DirectMemoryHeap {
		private:
			class Heap {
				friend class DirectMemoryHeap;
			private:
				off_t m_offset;
				size_t m_size;
				void *m_basePointer;
				SceLibcMspace m_mspace;
				int initialize(size_t alignment, size_t size, int memoryType, size_t maxPageSize);
				int finalize(void);
			public:
				void *allocate(size_t size);
				void *memalign(size_t boundary, size_t size);
				void free(void* ptr);
			};
		public:
			int initialize(size_t onionSize, size_t garlicSize);
			int finalize(void);

			Heap garlic;
			Heap onion;
		};
	};
};
