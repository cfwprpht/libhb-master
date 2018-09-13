/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/


#include "stdafx.h"
#include "psarc_util.h"
#include <assert.h>
#include <ctype.h>

#pragma comment( lib , "libSceFios2_stub_weak.a" )

using namespace common::Util;
namespace vm = sce::Vectormath::Simd::Aos;

int PsarcMount::mount(const std::string &archiveName, const std::string mountPoint) {
	SceFiosBuffer initFiosBuffer = SCE_FIOS_BUFFER_INITIALIZER;
	m_mountBuffer = initFiosBuffer;

	SceFiosPsarcDearchiverContext initDearchiverContext = SCE_FIOS_PSARC_DEARCHIVER_CONTEXT_INITIALIZER;
	m_dearchiverContext = initDearchiverContext;

	SceFiosSize result;
	int err;

	/*E Add a dearchiver I/O filter. Both the context struct and the work buffer must last for the lifetime of the dearchiver. */
	m_dearchiverContext.workBufferSize = sizeof(m_dearchiverWorkBuffer);
	m_dearchiverContext.pWorkBuffer = m_dearchiverWorkBuffer;
	m_archiveIndex = 0;
	err = sceFiosIOFilterAdd(m_archiveIndex, sceFiosIOFilterPsarcDearchiver, &m_dearchiverContext);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL( err, SCE_FIOS_OK );


	/*E Find out how much memory is needed to mount the archive. */
	result = sceFiosArchiveGetMountBufferSizeSync(NULL, archiveName.c_str(), NULL);
	if (result < 0) {
		return result;
	}

	/*E Allocate a mount buffer. */
	m_mountBuffer.length = (size_t)result;
	m_mountBuffer.pPtr = malloc(m_mountBuffer.length);
	//SCE_SAMPLE_UTIL_ASSERT(m_mountBuffer.pPtr != NULL);

	/*E Mount the archive. */
	m_archiveFH = -1;
	err = sceFiosArchiveMountSync(NULL, &m_archiveFH, archiveName.c_str(), mountPoint.c_str(), m_mountBuffer, NULL);
	//SCE_SAMPLE_UTIL_ASSERT_MSG(err == SCE_OK, "err=%#x\n", err);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL( sceFiosIsValidHandle(m_archiveFH), true );
	return SCE_OK;
}

int PsarcMount::unmount(void) {
	int err;
	/*E Unmount the archive. */
	err = sceFiosArchiveUnmountSync(NULL, m_archiveFH);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL( err, SCE_OK );

	/*E Free the mount buffer. */
	free(m_mountBuffer.pPtr);

	/*E Remove the dearchiver. */
	err = sceFiosIOFilterRemove(m_archiveIndex);
	//SCE_SAMPLE_UTIL_ASSERT_EQUAL( err, SCE_OK );
	/*E Prevent warnings in release builds. */

	/*E Terminate FIOS2. */
	sceFiosTerminate();
	return SCE_OK;
}