#ifndef BW_APPLICATION_COMMON_H
#define BW_APPLICATION_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../application.h"

typedef struct {
	bw_Application* app;
	bw_ApplicationReadyFn func;
	void* data;
} bw_ApplicationImpl_ReadyHandlerData;



BOOL bw_ApplicationImpl_dispatch( bw_Application* app, bw_ApplicationDispatchData* data );
void bw_ApplicationImpl_finish( bw_ApplicationImpl* );
int bw_ApplicationImpl_run( bw_Application* app, bw_ApplicationImpl_ReadyHandlerData* ready_handler_data );
bw_ApplicationImpl bw_ApplicationImpl_initialize( bw_Application* app, int argc, char** argv, const bw_ApplicationSettings* settings );

void bw_ApplicationEngineImpl_finish( bw_ApplicationEngineImpl* );
bw_ApplicationEngineImpl bw_ApplicationEngineImpl_initialize( bw_Application* app, int argc, char** argv, const bw_ApplicationSettings* settings );



#ifdef __cplusplus
} // extern "C"
#endif

#endif//BW_APPLICATION_COMMON_H
