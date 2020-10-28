#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")

#include "win32.h"
#include "../application.h"

#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <WinDef.h>
#include <Windows.h>

#include "../win32.h"
#include "../window/win32.h"



void bw_Application_dispatch( bw_Application* app, bw_ApplicationDispatchFn func, void* data ) {

	bw_ApplicationDispatchData* dispatch_data = malloc( sizeof( bw_ApplicationDispatchData ) );
	dispatch_data->func = func;
	dispatch_data->data = data;

	PostThreadMessageW( app->thread_id, WM_APP, (WPARAM)NULL, (LPARAM)dispatch_data );
}

bw_Application* bw_Application_new() {
	bw_Application* app = malloc( sizeof( bw_Application ) );

	app->thread_id = GetCurrentThreadId();
 	app->handle = GetModuleHandle( NULL );
	app->engine_data = 0;

	_bw_Window_init( app );

	return app;
}

void bw_Application_free( bw_Application* app ) {

	_bw_Application_uninit( app );

	UnregisterClassW( L"browser_window", app->handle );

	free( app );
}


void bw_Application_exit( bw_Application* app, int exit_code ) {
	// We assume the size of an int is smaller or equal to the size of a pointer.
	// This should be true for 32 and 64 bit systems in general.
	_STATIC_ASSERT( sizeof(int) <= sizeof(WPARAM) );

	PostThreadMessageW( app->thread_id, WM_QUIT, (WPARAM)exit_code, (LPARAM)NULL );
}

void bw_Application_exit_sync( bw_Application* app, int code ) {
	// PostThreadMessage is threadsafe, so we do exactly the same thing
	bw_Application_exit( app, code );
}

int bw_Application_run( bw_Application* app ) {
	MSG msg;

	while( true ) {
		BOOL res = GetMessageW( &msg, 0, 0, 0);

		if ( res == 0 ) {
			int exit_code = (int)msg.wParam;
			return exit_code;
		}
		else if (res == -1) {
			BW_WIN32_ASSERT_ERROR;
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessageW( &msg );

			// Execute the dispatch functions when given
			if ( msg.message == WM_APP ) {
				bw_ApplicationDispatchData* params = (bw_ApplicationDispatchData*)msg.lParam;

				(params->func)( app, params->data );

				free( params );
			}
			/*else if ( msg.message == WM_APP + 1 ) {
				bw_WindowDispatchData* params = (bw_WindowDispatchData*)msg.lParam;

				(params->func)( params->window, params->data );

				free( params );
			}*/
		}
	}
}