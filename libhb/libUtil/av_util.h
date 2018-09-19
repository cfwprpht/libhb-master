/* SCE CONFIDENTIAL
* PlayStation(R)4 Programmer Tool Runtime Library Release 03.508.031
* Copyright (C) 2013 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#pragma once

#pragma comment( lib , "BaseService_stub_weak")

#include <sampleutil.h>
#include <mspace.h>
#include <sceavplayer.h>
#include <video_recording.h>
#include "thread_util.h"


#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

namespace common {
	namespace Util {
		class PRX_INTERFACE AvPlayer {
		public:
			AvPlayer(void);
			virtual ~AvPlayer(void);

			int initialize(
				sce::SampleUtil::Graphics::GraphicsLoader *loader,
				sce::SampleUtil::Graphics::SpriteRenderer *spriteRenderer,
				sce::SampleUtil::Audio::AudioContext *audioContext,
				DirectMemoryHeap *heap
				);
			int finalize(void);
			int update(void);
			void render(sce::SampleUtil::Graphics::GraphicsContext *context, sce::SampleUtil::Graphics::SpriteRenderer *sprite);

			int start(const char* fileName);
			int stop(void);
			bool isPlaying(void);

		private:
			static void eventCallback(void* p, int32_t argEventId, int32_t argSourceId, void* argEventData);

			sce::SampleUtil::Graphics::GraphicsLoader *m_loader;
			sce::SampleUtil::Graphics::SpriteRenderer *m_spriteRenderer;
			sce::SampleUtil::Audio::AudioContext *_audioContext;
			DirectMemoryHeap *m_heap;

			SceAvPlayerHandle						m_hSamplePlayer;
			sce::SampleUtil::Audio::Voice *m_rawVoice;

			sce::SampleUtil::Graphics::Texture2dBuffer *m_yBuffer;
			sce::SampleUtil::Graphics::Texture2dBuffer *m_cbcrBuffer;
			sce::SampleUtil::Graphics::Texture*		m_y;
			sce::SampleUtil::Graphics::Texture*		m_cbcr;

			static void* audioThreadEntry(void *arg);
			common::Util::Thread m_audioThread;

			uint32_t m_width;
			uint32_t m_height;

			uint64_t m_videoTimeStamp;
			uint64_t m_audioTimeStamp;
			uint64_t m_startTime;

			struct TexPair {
				sce::SampleUtil::Graphics::Texture*		y;
				sce::SampleUtil::Graphics::Texture*		cbcr;
			};
			struct TextureBuffer {
			private:
				uint32_t m_numBuffer;
				TexPair *m_pairs;
				int32_t m_index;
				uint32_t m_width;
				uint32_t m_height;

			public:
				TextureBuffer(void) { m_pairs = NULL; }
				~TextureBuffer(void) { finalize(); }
				int initialize(sce::SampleUtil::Graphics::GraphicsLoader *loader, uint32_t numBuffer,  uint32_t width, uint32_t height);
				int finalize(void);
				void pushData(const void *yData, const void *cbcrData);
				TexPair getCurrentTexture(void);
			};
			
			struct AudioData {
				void* data;
				uint32_t size;
			};

			TextureBuffer m_textureBuffer;
			AudioData m_pendingAudioData;
			bool m_isStarted;
			bool m_isPlaying;
		};


	}
}
