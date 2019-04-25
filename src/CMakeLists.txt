cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)
#add_definitions(-D_OpenGL_Renderer_)
add_definitions(-DGLTF)
add_definitions(-DTINYGLTF_ANDROID_LOAD_FROM_ASSETS)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)

find_library(log-lib
        log)


set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(maincode
    SHARED

    main.cpp
    #opengl_renderer.cpp
    3D_objects.cpp
        model_loader.cpp
        asset_manager.cpp
        engine.cpp
        WindowManager.cpp
        Game/ThirdPerson.cpp
        Game/gui.cpp
        camera.cpp
    )


add_subdirectory(glm)

target_link_libraries(maincode
        android
        native_app_glue
        ${log-lib}
        EGL
        GLESv2
        

        )