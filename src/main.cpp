
#ifdef WINDOWS
#define STB_IMAGE_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN 
#pragma comment(linker, "/subsystem:windows")
#pragma warning(disable:4996)

#include "engine.h"
#include <Windows.h>
#include <tchar.h>
//int main() {
//	std::cout << "Engine\n";
//	Engine engine;
//
//	engine.init();
//	engine.textures_manager.free_textures_from_cpu_memory(&engine,engine.meshes);
//	
//
//
//	engine.main_loop();
//    return 1;
//}

LRESULT CALLBACK WndProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#ifdef DEVELOPMENT
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
#endif // DEVELOPMENT

	



	LPCSTR title = "new title";
	LPCSTR clas_name = "engine";
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = clas_name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}	


	HWND hWnd = CreateWindow(
		wcex.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}


	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);


	

	Engine engine;
	//engine.init();
	engine.renderer.init();
	//engine.textures_manager.free_textures_from_cpu_memory(&engine, engine.meshes);
	//engine.main_loop();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		engine.renderer.draw_frame();
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}
#endif

#if defined (ES2) || defined (VULKAN) || defined (ANDROID)
    #define STB_IMAGE_IMPLEMENTATION
    #include "engine.h"
#endif

#if defined (LINUX) && defined (ES2) || defined (VULKAN) 
    
   
        int main() {
            
            Engine engine;
            engine.init();
			engine.textures_manager.free_textures_from_cpu_memory(&engine, engine.meshes);
            engine.main_loop();                  

            return EXIT_SUCCESS;      
        }
   
   
#endif

#ifdef ANDROID
    #include "engine.h"
#include <android/input.h>
    extern "C" {

    int32_t handle_input(android_app* app, AInputEvent* event) {
        auto *engine = reinterpret_cast<Engine *>(app->userData);

        int32_t eventType = AInputEvent_getType(event);
        switch(eventType){
            case AINPUT_EVENT_TYPE_MOTION:
                switch(AInputEvent_getSource(event)){
                    case AINPUT_SOURCE_TOUCHSCREEN:
                        int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                        switch(action){
                            case AMOTION_EVENT_ACTION_DOWN:
                                engine->input.left_button_pressed = true;
                                engine->input.input_event(event);
                                break;
                            case AMOTION_EVENT_ACTION_UP:
                                engine->input.left_button_pressed = false;
                                engine->input.left_button_release = true;

                                break;
                            case AMOTION_EVENT_ACTION_MOVE:
                                engine->input.left_button_pressed = true;
                                engine->input.input_event(event);
                                break;
                        }
                        break;
                } // end switch
                break;
            case AINPUT_EVENT_TYPE_KEY:
                // handle key input...
                break;
        } // end switch
        return  -1;
    }

    void handle_cmd(android_app *pApp, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                    
                    pApp->userData = new Engine(pApp);
                                        
                    break;




            case APP_CMD_TERM_WINDOW:
                if (pApp->userData) {
                    auto *pEngine = reinterpret_cast<Engine *>(pApp->userData);
                    pApp->userData = nullptr;
                    delete pEngine;
                }
        }
    }

    void android_main(struct android_app *pApp) {
        pApp->onAppCmd = handle_cmd;
        pApp->onInputEvent = handle_input;

        int events;
        android_poll_source *pSource;
        while(!pApp->userData){

                if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
                    if (pSource) {
                        pSource->process(pApp, pSource);
                    }
                }
        }
        if (pApp->userData) {
            auto *engine = reinterpret_cast<Engine *>(pApp->userData);
            engine->init();
            engine->main_loop();
        }

    }
    }
#endif