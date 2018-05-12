/* SCE CONFIDENTIAL
 * PlayStation(R)4 Programmer Tool Runtime Library Release 03.508.031
 * Copyright (C) 2013 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

#include "stdafx.h"
#include <stdio.h>
#include <kernel.h>
#include <scebase_common.h>
#include <libsysmodule.h>
#include "av_util.h"
#include "sceavplayer_ex.h"
#include <ngs2.h>

using namespace common::Util;
namespace vecmath = sce::Vectormath::Simd::Aos;
namespace ssg = sce::SampleUtil::Graphics;

int AvPlayer::TextureBuffer::initialize(ssg::GraphicsLoader *loader, uint32_t numBuffer,  uint32_t width, uint32_t height) {
	int ret;
	(void)ret;
	m_numBuffer = numBuffer;
	m_pairs = new TexPair[numBuffer];
	for(uint32_t i=0; i<numBuffer; i++){
		ret = loader->createTexture(	&m_pairs[i].y,
			ssg::kTextureFormatR8Unorm,
			width, height, 1 );
		SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
		
		ret = loader->createTexture(	&m_pairs[i].cbcr,
			ssg::kTextureFormatG8R8Unorm,
			width/2, (height/2), 1 );
		SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
	}
	m_index = -1;
	m_width = width;
	m_height = height;
				

	return SCE_OK;
}

int AvPlayer::TextureBuffer::finalize(void) {
	if(m_pairs != NULL){
		for(uint32_t i=0; i<m_numBuffer; i++){
			if(m_pairs[i].y != NULL){
				sce::SampleUtil::destroy(m_pairs[i].y);
				m_pairs[i].y = NULL;
			}
			if(m_pairs[i].cbcr != NULL){
				sce::SampleUtil::destroy(m_pairs[i].cbcr);
				m_pairs[i].cbcr = NULL;
			}
		}
		delete [] m_pairs;
		m_pairs = NULL;
	}
	return SCE_OK;
}

static void fast_memcpy(void *dst, const void* src, size_t size) {
	//printf("memcpy(%p, %p, %lx\n", dst, src, size);

	uint64_t *dst64 =(uint64_t *)dst;
	const uint64_t *src64 =(const uint64_t *)src;

	size_t times = size / (sizeof(uint64_t) * 16);
	for(size_t i=0; i<times; i++){
		uint64_t d0 = src64[i*16+0x0];
		uint64_t d1 = src64[i*16+0x1];
		uint64_t d2 = src64[i*16+0x2];
		uint64_t d3 = src64[i*16+0x3];

		uint64_t d4 = src64[i*16+0x4];
 		uint64_t d5 = src64[i*16+0x5];
		uint64_t d6 = src64[i*16+0x6];
		uint64_t d7 = src64[i*16+0x7];

		uint64_t d8 = src64[i*16+0x8];
		uint64_t d9 = src64[i*16+0x9];
		uint64_t da = src64[i*16+0xa];
		uint64_t db = src64[i*16+0xb];

		uint64_t dc = src64[i*16+0xc];
		uint64_t dd = src64[i*16+0xd];
		uint64_t de = src64[i*16+0xe];
		uint64_t df = src64[i*16+0xf];

		dst64[i*16+0x0] = d0;
		dst64[i*16+0x1] = d1;
		dst64[i*16+0x2] = d2;
		dst64[i*16+0x3] = d3;

		dst64[i*16+0x4] = d4;
 		dst64[i*16+0x5] = d5;
		dst64[i*16+0x6] = d6;
		dst64[i*16+0x7] = d7;

		dst64[i*16+0x8] = d8;
		dst64[i*16+0x9] = d9;
		dst64[i*16+0xa] = da;
		dst64[i*16+0xb] = db;

		dst64[i*16+0xc] = dc;
		dst64[i*16+0xd] = dd;
		dst64[i*16+0xe] = de;
		dst64[i*16+0xf] = df;
	}
}

void AvPlayer::TextureBuffer::pushData(const void *yData, const void *cbcrData) {
	if(m_index < 0) {
		m_index = 0;
	} else {
		m_index = (m_index+1) % m_numBuffer;
	}
	TexPair pair = m_pairs[m_index];
	{
		void *yP = pair.y->beginWrite();
		fast_memcpy(yP, yData, pair.y->getStride() * m_height);
		pair.y->endWrite();
	}
	{
		void *cbcrP = pair.cbcr->beginWrite();
		fast_memcpy(cbcrP, cbcrData, pair.cbcr->getStride() * m_height);
		pair.cbcr->endWrite();
	}
}

AvPlayer::TexPair AvPlayer::TextureBuffer::getCurrentTexture(void) {
	if(m_index < 0) {
		TexPair p;
		p.y = NULL;
		p.cbcr = NULL;
		return p;
	}
	return m_pairs[m_index];
}


/* --------------------------------------------------------------------- */
AvPlayer::AvPlayer(void) {
	m_hSamplePlayer		= NULL;	
	m_yBuffer = NULL;
	m_cbcrBuffer = NULL;
	m_y = NULL;
	m_cbcr = NULL;	
}

AvPlayer::~AvPlayer(void) {}

int AvPlayer::initialize(ssg::GraphicsLoader *loader, ssg::SpriteRenderer *spriteRenderer, sce::SampleUtil::Audio::AudioContext *audioContext, DirectMemoryHeap *heap ) {
	int ret;
	(void)ret;
	ret = sceSysmoduleLoadModule(SCE_SYSMODULE_AV_PLAYER);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );

	m_heap           = heap;
	m_loader         = loader;
	m_spriteRenderer = spriteRenderer;
	m_audioContext   = audioContext;
	m_heap           = heap;
	m_isStarted      = false;
	m_videoTimeStamp = 0;
	m_audioTimeStamp = 0;

	return SCE_OK;
}

int AvPlayer::finalize(void) {
	int ret;
	ret = sceSysmoduleUnloadModule(SCE_SYSMODULE_AV_PLAYER);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
	return SCE_OK;
}

void AvPlayer::eventCallback(void* p, int32_t argEventId, int32_t argSourceId, void* argEventData) {
	int ret;
	(void)ret;
	AvPlayer *self = (AvPlayer*)p;
	if(argEventId == SCE_AVPLAYER_STATE_READY) {
		printf("TIMESTAMP0: %ld----------------------------------------\n",  sceAvPlayerCurrentTime(self->m_hSamplePlayer));

		int numStream = sceAvPlayerStreamCount(self->m_hSamplePlayer);
		for(uint32_t i=0;i<numStream; i++){
			SceAvPlayerStreamInfo streamInfo;
			memset(&streamInfo, 0, sizeof(streamInfo));

			ret =  sceAvPlayerGetStreamInfo(self->m_hSamplePlayer, i, &streamInfo);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

			if(streamInfo.type == SCE_AVPLAYER_AUDIO) {
				ret = self->m_audioContext->createRawVoice(&self->m_rawVoice, 1024*128, SCE_NGS2_WAVEFORM_TYPE_PCM_I16L, 
					streamInfo.details.audio.channelCount, streamInfo.details.audio.sampleRate);
				SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
				ret = self->m_rawVoice->play();
				SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

				printf("enbale audio\n");
				ret = sceAvPlayerEnableStream(self->m_hSamplePlayer, i);
				SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
			}
			if (streamInfo.type == SCE_AVPLAYER_VIDEO) {
				ret = self->m_textureBuffer.initialize(self->m_loader, 3, streamInfo.details.video.width, streamInfo.details.video.height);
				SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
				
				self->m_width  = streamInfo.details.video.width;
				self->m_height = streamInfo.details.video.height;

				printf("enbale video\n");
				ret = sceAvPlayerEnableStream(self->m_hSamplePlayer, i);
				SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
			}
		}
		printf("TIMESTAMP0: %ld\n", sceAvPlayerCurrentTime(self->m_hSamplePlayer));
		printf("start\n");

		self->m_startTime = sceKernelGetProcessTime();	
		ret = sceAvPlayerStart(self->m_hSamplePlayer);
		SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
		self->m_isStarted = true;
	}
}

static void* allocateCallback( void* arg, uint32_t alignment, uint32_t size ) {
	common::Util::DirectMemoryHeap* heap		= (common::Util::DirectMemoryHeap*) arg;
	return heap->onion.memalign(alignment, size);
}

static void deallocateCallback( void* arg, void* pMemory ) {
	common::Util::DirectMemoryHeap* heap		= (common::Util::DirectMemoryHeap*) arg;
	heap->onion.free(pMemory);
}

static void* allocateTextureCallback( void* arg, uint32_t alignment, uint32_t size ) {
	common::Util::DirectMemoryHeap* heap		= (common::Util::DirectMemoryHeap*) arg;
	return heap->garlic.memalign(alignment, size);
}

static void deallocateTextureCallback( void* arg, void* pMemory ) {
	common::Util::DirectMemoryHeap* heap		= (common::Util::DirectMemoryHeap*) arg;
	return heap->garlic.free(pMemory);
}

void* AvPlayer::audioThreadEntry(void *arg) {
//	return NULL;
	AvPlayer *self = (AvPlayer*)arg;
	static const uint32_t bufSize = 64*1024;
	uint8_t *buf = new uint8_t[bufSize];

	while(1) {
		if (!self->m_isPlaying) break;
		if (!self->m_isStarted) {
			sceKernelUsleep(1000);
			continue;
		}
		int ret;
/*		
		SceAvPlayerFrameInfo audioFrame;

		uint32_t offset=0;
		while(1){
			memset( &audioFrame, 0, sizeof(SceAvPlayerFrameInfo) );
			bool audioAvailable = sceAvPlayerGetAudioData( self->m_hSamplePlayer, &audioFrame );
			if(!audioAvailable){
				//printf("audio not available\n");
				sceKernelUsleep(100);
				continue;
			}
			memcpy(&buf[offset], audioFrame.pData, audioFrame.details.audio.size);
			offset += audioFrame.details.audio.size;
			if(offset >= bufSize/2){
				break;
			}
		}
*/
#if 0
		while(1){
			memset( &audioFrame, 0, sizeof(SceAvPlayerFrameInfo) );
			bool audioAvailable = sceAvPlayerGetAudioData( self->m_hSamplePlayer, &audioFrame );
			if(!audioAvailable){
				sceKernelUsleep(100);

				if(!self->m_isPlaying){
					return NULL;
				}
				continue;
			}
			printf("audio time stamp=%ld\n", audioFrame.timeStamp);
			break;
		}

		while(1){
			uint64_t currentTime = sceKernelGetProcessTime() - self->m_startTime; 
			if(audioFrame.timeStamp >= (currentTime/1000   + 1000/60)){
				sceKernelUsleep(100);
				if(!self->m_isPlaying){
					return NULL;
				}

				continue;
			}
			ret = self->m_rawVoice->addData(audioFrame.pData, audioFrame.details.audio.size);
			if(ret != SCE_OK){
				sceKernelUsleep(100);
				if(!self->m_isPlaying){
					return NULL;
				}

				continue;
			}
			break;
		}
#endif

		static int isFirst = 0;
		do {
		if(self->m_pendingAudioData.data != NULL){
			ret = self->m_rawVoice->addData(self->m_pendingAudioData.data, self->m_pendingAudioData.size);
			if(ret != SCE_OK){
				//printf("audio buffer is full\n");
				break;
			}
			//printf("audio buffer pushed\n");

			self->m_pendingAudioData.data = NULL;
			self->m_pendingAudioData.size = 0;
			isFirst = 1;

			//to audioは取得したのをすぐ出力することを想定しているので連続で取らないようにする。なのでbreakする。
			//to 問題なければgetaudiodataで取得できたら突っ込んで終わりとするコードのほうが素直かも
			break;
		}
		SceAvPlayerFrameInfo audioFrame;
		memset( &audioFrame, 0, sizeof(SceAvPlayerFrameInfo) );
		bool audioAvailable = sceAvPlayerGetAudioData( self->m_hSamplePlayer, &audioFrame );
		if(!audioAvailable){
			//printf("audio data is not available\n");
			break;
		}

		self->m_pendingAudioData.data = audioFrame.pData;
		self->m_pendingAudioData.size = audioFrame.details.audio.size;
		} while (1);

		//to 1024サンプル出てくるので 1024/48000で待つ時間がわかる。サイズから取得する場合はsizeをchで割ればいいはず
		//to 通常はこれだけ待つようにする。audioデータ失敗のときもこれくらい待ってもよい。というのは始まるまでに
		//to ちゃんとavplayer側に操作権がわたらないと開始できないので
		//to あとaudioのスレッドのpriorityはほかよりちょっと上げてほしい(現状700)
		if (!isFirst) sceKernelUsleep(21.3*1000.0);
		else {
			//to 1回めだけあえて待たないようにする。avplayer内部でも多少余裕もっているので、これでaudioのバッファがなくなることがないはず。
			isFirst = 0;
			//to ビジーループ回避
			sceKernelUsleep(100);
		}
	}
	delete [] buf;
	return NULL;
}

int AvPlayer::start(const char* fileName ) {
	int ret;
	(void)ret;

	if ( m_hSamplePlayer != NULL ) return -1;
	m_pendingAudioData.data = NULL;
	m_pendingAudioData.size = 0;


	SceAvPlayerInitData playerInit;
	memset(&playerInit, 0, sizeof(SceAvPlayerInitData));
	playerInit.memoryReplacement.objectPointer     = m_heap;
	playerInit.memoryReplacement.allocate          = allocateCallback;
	playerInit.memoryReplacement.deallocate        = deallocateCallback;
	playerInit.memoryReplacement.allocateTexture   = allocateTextureCallback;
	playerInit.memoryReplacement.deallocateTexture = deallocateTextureCallback;
	playerInit.eventReplacement.objectPointer      = this;
	playerInit.eventReplacement.eventCallback      = eventCallback;
	playerInit.autoStart                           = false;
	playerInit.numOutputVideoFrameBuffers          = 2;
	//playerInit.debugLevel = SCE_AVPLAYER_DBG_ALL;
	playerInit.debugLevel                          = SCE_AVPLAYER_DBG_NONE;
	playerInit.basePriority                        = 0;
	m_isStarted                                    = false;
	m_hSamplePlayer                                = sceAvPlayerInit(&playerInit);
	SCE_SAMPLE_UTIL_ASSERT(m_hSamplePlayer != NULL);

	SceAvPlayerPostInitData playerPostInit;
	memset(&playerPostInit, 0, sizeof(SceAvPlayerPostInitData));

	playerPostInit.demuxVideoBufferSize = (8 * 1024 * 1024);
	playerPostInit.videoDecoderInit.decoderType.videoType = SCE_AVPLAYER_VIDEO_DECODER_TYPE_SOFTWARE2;
	playerPostInit.videoDecoderInit.decoderParams.avcSw2.decodePipelineDepth = 6;
	playerPostInit.videoDecoderInit.decoderParams.avcSw2.computePipeId  = 0;
	playerPostInit.videoDecoderInit.decoderParams.avcSw2.computeQueueId = 0;
	ret = sceAvPlayerPostInit(m_hSamplePlayer, &playerPostInit);
	SCE_SAMPLE_UTIL_ASSERT(ret == 0);

	sceAvPlayerStop(m_hSamplePlayer);
	m_isPlaying = true;

	ret = sceAvPlayerAddSource(m_hSamplePlayer, fileName);
	if (ret != SCE_OK) {
		sceAvPlayerClose(m_hSamplePlayer);
		m_hSamplePlayer		= NULL;
		m_isStarted = false;
		m_isPlaying = false;
		return ret;
	}

	ret = m_audioThread.start("audiothread", audioThreadEntry, this);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );

	//while(!m_isStarted){
	//sceKernelUsleep(1000);
//}
	return SCE_OK;
}

int AvPlayer::stop(void) {
	int ret;
	(void)ret;

	if (!m_isPlaying) return SCE_OK;
	m_isPlaying = false;

	if ( m_hSamplePlayer != NULL ) {
		sceAvPlayerStop( m_hSamplePlayer );
		sceAvPlayerClose( m_hSamplePlayer );
		m_hSamplePlayer		= NULL;
	}

	ret = m_audioThread.join(NULL);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );

	if (m_yBuffer!= NULL) {
		sce::SampleUtil::destroy(m_yBuffer);
		m_yBuffer = NULL;
	}
	if (m_cbcrBuffer!= NULL) {
		sce::SampleUtil::destroy(m_cbcrBuffer);
		m_cbcrBuffer = NULL;
	}

	if (m_y!= NULL) {
		sce::SampleUtil::destroy(m_y);
		m_y= NULL;
	}
	if (m_cbcr!= NULL) {
		sce::SampleUtil::destroy(m_cbcr);
		m_cbcr= NULL;
	}
	ret = m_textureBuffer.finalize();
	SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );

	m_pendingAudioData.data = NULL;
	m_pendingAudioData.size = 0;

	if (m_rawVoice != NULL) {
		sce::SampleUtil::destroy(m_rawVoice);
		m_rawVoice = NULL;
	}
	m_isStarted = false;
	return SCE_OK;
}


int AvPlayer::update(void) {
	if (!m_isPlaying) return SCE_OK;

	if(!m_isStarted) return SCE_OK;
	int ret;

#if 0
	do{
		if(m_pendingAudioData.data != NULL){
			ret = m_rawVoice->addData(m_pendingAudioData.data, m_pendingAudioData.size);
			if(ret != SCE_OK){
				//printf("audio buffer is full\n");
				break;
			}
			//printf("audio buffer pushed\n");
			m_pendingAudioData.data = NULL;
			m_pendingAudioData.size = 0;
			break;
		}
		SceAvPlayerFrameInfo audioFrame;
		memset( &audioFrame, 0, sizeof(SceAvPlayerFrameInfo) );
		bool audioAvailable = sceAvPlayerGetAudioData( m_hSamplePlayer, &audioFrame );
		if(!audioAvailable){
			//printf("audio data is not available\n");
			break;
		}

		m_pendingAudioData.data = audioFrame.pData;
		m_pendingAudioData.size = audioFrame.details.audio.size;
	}while(1);
#endif


	while (1) {
		SceAvPlayerFrameInfo videoFrame;
		memset(&videoFrame, 0, sizeof(SceAvPlayerFrameInfo));
		
		bool frameAvailable = sceAvPlayerGetVideoData(m_hSamplePlayer, &videoFrame);
		//printf("TIMESTAMP: a=%d, %ld, %ld\n",  frameAvailable, sceAvPlayerCurrentTime(m_hSamplePlayer), videoFrame.timeStamp);
		if (frameAvailable) {
			m_videoTimeStamp = videoFrame.timeStamp;
			//printf("video time stamp=%ld\n", videoFrame.timeStamp);
	
			void *y = videoFrame.pData;
			void *cbcr = (((uint8_t*)videoFrame.pData) + (videoFrame.details.video.width * videoFrame.details.video.height));
			
			if (m_yBuffer != NULL) {
				sce::SampleUtil::destroy(m_yBuffer);
				m_yBuffer = NULL;
			}

			if (m_cbcrBuffer != NULL) {
				sce::SampleUtil::destroy(m_cbcrBuffer);
				m_cbcrBuffer = NULL;
			}
			
			if (m_y != NULL) {
				sce::SampleUtil::destroy(m_y);
				m_y= NULL;
			}

			if (m_cbcr != NULL) {
				sce::SampleUtil::destroy(m_cbcr);
				m_cbcr= NULL;
			}
			
			ret = m_loader->createTexture2dBufferUsingExistentBuffer(&m_yBuffer, ssg::kBufferFormatX8, m_width, m_height,1,
				ssg::kBufferAccessModeGpuReadCpuWrite,
				ssg::kBufferBindFlagShaderResource,
				ssg::kMultisampleNone, y);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
			SCE_SAMPLE_UTIL_ASSERT(m_yBuffer != NULL);
			ret = m_loader->createTexture2dBufferUsingExistentBuffer(&m_cbcrBuffer, ssg::kBufferFormatX8X8, m_width/2, m_height/2,1,
				ssg::kBufferAccessModeGpuReadCpuWrite,
				ssg::kBufferBindFlagShaderResource,
				ssg::kMultisampleNone, cbcr);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
			SCE_SAMPLE_UTIL_ASSERT(m_cbcrBuffer != NULL);
			
			ret = m_loader->createTextureFromBuffer(&m_y, m_yBuffer, ssg::kTextureFormatR8Unorm);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
			SCE_SAMPLE_UTIL_ASSERT(m_y != NULL);
			
			ret = m_loader->createTextureFromBuffer(&m_cbcr, m_cbcrBuffer, ssg::kTextureFormatG8R8Unorm);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL( ret, SCE_OK );
			SCE_SAMPLE_UTIL_ASSERT(m_cbcr != NULL);
			break;
		}
		break;
	}

#if 0
	do{
		if(m_pendingAudioData.data != NULL){
			if(m_audioTimeStamp > (m_videoTimeStamp + 1000/60) ){
				break;
			}
			ret = m_rawVoice->addData(m_pendingAudioData.data, m_pendingAudioData.size);
			if(ret != SCE_OK){
				//printf("audio buffer is full\n");
				break;
			}
			//printf("audio buffer pushed\n");
			m_pendingAudioData.data = NULL;
			m_pendingAudioData.size = 0;
		}
		SceAvPlayerFrameInfo audioFrame;
		memset( &audioFrame, 0, sizeof(SceAvPlayerFrameInfo) );
		bool audioAvailable = sceAvPlayerGetAudioData( m_hSamplePlayer, &audioFrame );
		if(!audioAvailable){
			//printf("audio data is not available\n");
			break;
		}
		m_pendingAudioData.data = audioFrame.pData;
		m_pendingAudioData.size = audioFrame.details.audio.size;
		m_audioTimeStamp = audioFrame.timeStamp;
		
	}while(1);
#endif			
	return SCE_OK;
}

void AvPlayer::render(sce::SampleUtil::Graphics::GraphicsContext *context, sce::SampleUtil::Graphics::SpriteRenderer *sprite) {
	context->setCullMode(ssg::kCullNone);
	context->clearRenderTarget(0x00000000);
	context->setDepthWriteEnable(true);
	context->setDepthFunc( ssg::kDepthFuncAlways );

	if (!m_isPlaying) return;
	if (!m_isStarted) return;

	if ((m_y != NULL) && (m_cbcr != NULL)) {
		m_spriteRenderer->drawTextureYcbcr(context, vecmath::Vector2(0.0f, 0.0f), vecmath::Vector2(1.0f, 1.0f), m_y, m_cbcr);
	}
}

bool AvPlayer::isPlaying(void) { return m_isPlaying; }
/* --------------------------------------------------------------------- */

