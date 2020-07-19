#include "Window.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <GL/gl.h>


void CheckSDLError(int line = -1) {
    std::string error = SDL_GetError();

    if (!error.empty()) {
        std::cout << "SLD Error : " << error << std::endl;

        if (line != -1)
            std::cout << "\nLine : " << line << std::endl;

        SDL_ClearError();
    }
}

bool SetOpenGLAttributes() {
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    return true;
}

bool loadGlad() {


    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }
    int a;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS,&a);
    printf("OpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));
    printf("GL_MAX_ARRAY_TEXTURE_LAYERS --> %i",a);
    return true;

}

SDL_Window *Window::create_window() {
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to init SDL\n";
        return nullptr;
    }

// Create our window centered at 512x512 resolution
    SDL_Window *mainwindow = SDL_CreateWindow(
            "Test",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screen_width,
            screen_height,
            SDL_WINDOW_OPENGL
    );
    if (!mainwindow) {
        std::cout << "Unable to create window\n";
        CheckSDLError(__LINE__);
        return nullptr;
    }

    // Create our opengl context and attach it to our window
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainwindow);

    loadGlad();

    SetOpenGLAttributes();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int w,h;
    SDL_GetWindowSize(mainwindow, &w, &h);
    glViewport(0, 0, w, h);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    return mainwindow;
}

Window::Window(int screen_width, int screen_height) : screen_width(screen_width), screen_height(screen_height) {}

int Window::getScreen_width() const {
    return screen_width;
}

void Window::setScreen_width(int screen_width) {
    Window::screen_width = screen_width;
}

int Window::getScreen_height() const {
    return screen_height;
}

void Window::setScreen_height(int screen_height) {
    Window::screen_height = screen_height;
}

bool Window::isM_Fullscreen() const {
    return m_Fullscreen;
}

void Window::setM_Fullscreen(bool m_Fullscreen) {
    Window::m_Fullscreen = m_Fullscreen;
}



