#include <SDL_image.h>
#include <SDL_opengl.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl2.h>

#include "./config.h"
#include "src/Gfx.h"
#include "src/Rt.h"

#include "assets/Font.h"

void loop() {
    while (RT_Running) {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                RT_Stop();
            }
        }

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(Gfx_Window);
        ImGui::NewFrame();
        {
        }
        ImGui::Render();

        // Flush
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(Gfx_Window);
        glFinish();
    }
}

void initImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui_ImplSDL2_InitForOpenGL(Gfx_Window, Gfx_GL_Context);
    ImGui_ImplOpenGL2_Init();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(assets_Font(), 16, 16);
}

void initSdl() {
    auto sdlFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS;

    if (SDL_Init(sdlFlags) != 0) {
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetSwapInterval(1);

    Gfx_Window = SDL_CreateWindow(
            configWindowTitle,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            configWindowWidth,
            configWindowHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );

    Gfx_Renderer = SDL_CreateRenderer(Gfx_Window, -1, SDL_RENDERER_ACCELERATED);
    Gfx_GL_Context = SDL_GL_CreateContext(Gfx_Window);
    SDL_GL_MakeCurrent(Gfx_Window, Gfx_GL_Context);
}

int main(int, char **) {
    initSdl();
    initImgui();
    loop();
}
