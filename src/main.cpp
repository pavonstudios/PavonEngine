
#ifdef WINDOWS
#define STB_IMAGE_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN 
#pragma comment(linker, "/subsystem:windows")
#pragma warning(disable:4996)

#include "engine.h"
#include <Windows.h>
#include <tchar.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

#ifdef DEVELOPMENT
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
#endif // DEVELOPMENT

	Engine engine;
	engine.window_manager.engine = &engine;
	engine.window_manager.create_window_windows(hInstance);


	ShowWindow(engine.window_manager.window_handler,
		nCmdShow);
	UpdateWindow(engine.window_manager.window_handler);

#ifdef VULKAN
	engine.init();
	while (!engine.window_manager.window_should_close()) {
		engine.window_manager.check_events();
		engine.renderer.draw_frame();
	}
#endif
	
#ifdef OPENGL
	engine.init();
	engine.textures_manager.free_textures_from_cpu_memory(&engine, engine.meshes);
	engine.main_loop();
	while (!engine.window_manager.window_should_close()) {
		engine.window_manager.check_events();
		glClearColor(0.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(engine.window_manager.device_context);
	}
#endif

#ifdef DX11
	engine.init();
	engine.textures_manager.free_textures_from_cpu_memory(&engine, engine.meshes);
	engine.main_loop();
#endif // DX11

#ifdef DX12
	engine.renderer.engine = &engine;
	engine.renderer.init();
	while (!engine.window_manager.window_should_close()) {
		engine.window_manager.check_events();
		engine.renderer.draw_frame();
	}
#endif
	
	return 1;
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