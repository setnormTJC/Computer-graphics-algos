//#define _USE_MATH_DEFINES
//#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
//
//#include "Camera.h"
//#include "Cube.h"
//#include "Mesh.h"
//#include <thread>
//
//
//constexpr int TARGET_FPS = 1;
//constexpr int FRAME_DELAY_MS = 1000 / TARGET_FPS;
//
//static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;
//
///* This function runs once at startup. */
//SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
//{
//    /* Create the window */
//    if (!SDL_CreateWindowAndRenderer("Cubey", 500, 500, 0 , &window, &renderer)) {
//        //set 4th arg to SDL_WINDOW_FULLSCREEN, if desired
//        //0 evidently makes window size dependent on the width and height (2nd and 3rd args)
//        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
//        return SDL_APP_FAILURE;
//    }
//    return SDL_APP_CONTINUE;
//}
//
///* This function runs when a new event (mouse input, keypresses, etc) occurs. */
//SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
//{
//    if (event->type == SDL_EVENT_KEY_DOWN ||
//        event->type == SDL_EVENT_QUIT) {
//        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
//    }
//    return SDL_APP_CONTINUE;
//}
//
///* This function runs once per frame, and is the heart of the program. */
//SDL_AppResult SDL_AppIterate(void* appstate)
//{
//    static int loopCount = 0;
//    constexpr int screenWidth = 500;
//    constexpr int screenHeight = 500;
//
//
//    try
//    {
//        auto frameStart = std::chrono::high_resolution_clock::now();
//
//        Cube cube{};
//
//        std::vector<Vec4> localCubeVerts = cube.getLocalCubeVerts();
//
//        Vec4 rot(M_PI / 6.0f * loopCount, M_PI / 5.0f * loopCount, M_PI / 4.0f * loopCount, 0.0f); //units: radians, rotate about y-axis 45 degrees (w = 0.0f here?) 
//        Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f); //again, not sure if w = 0.0f or 1.0f here 
//        Vec4 trans(0.0f, 0.0f, -3.0f, 1.0f); //ensure that z < 0.0f  (default camera pos) for zInit + zTrans
//
//        Mesh mesh(localCubeVerts, trans, rot, scale);
//
//        std::vector<Vec4> worldCubeVerts = mesh.applyModelMatrix();
//
//        float fovY = (M_PI / 2);
//        Camera camera(fovY);
//
//        auto screenSpaceCubeVerts = camera.projectToScreen(worldCubeVerts, screenWidth, screenHeight);
//
//        std::vector<Color> colors;
//        auto colorEnums = Color::getBroadColorPalette(); 
//        for (const auto& current : colorEnums)
//        {
//            colors.push_back(Color(current));
//        }
//
//        auto rasteredPixels = cube.filledRasterize(screenSpaceCubeVerts, colors);
//
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//        SDL_RenderClear(renderer);
//
//        for (auto& [pos, color] : rasteredPixels)
//        {
//            Uint8 r, g, b;
//            color.getRGB8(r, g, b);
//            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
//
//            SDL_RenderPoint(renderer, pos.x, pos.y);
//
//            //SDL_GetRenderDrawColor(renderer, color.getR8(), )
//        }
//        SDL_RenderPresent(renderer);
//        std::cout << "Draw it, draw it real good\n";
//
//        auto frameEnd = std::chrono::high_resolution_clock::now();
//        int frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
//
//        if (frameTime < FRAME_DELAY_MS)
//            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY_MS - frameTime));
//
//        loopCount++;
//
//        return SDL_APP_CONTINUE;
//    }
//
//    catch (const MyException& e)
//    {
//        std::cout << e.whatWentWrong();
//        return SDL_APP_FAILURE;
//    }
//
//    ///* Center the message and scale it up */
//    //SDL_GetRenderOutputSize(renderer, &w, &h);
//    //SDL_SetRenderScale(renderer, scale, scale);
//    //x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
//    //y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;
//
//    ///* Draw the message */
//    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    //SDL_RenderClear(renderer);
//    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    //SDL_RenderDebugText(renderer, x, y, message);
//    //SDL_RenderPresent(renderer);
//
//}
//
///* This function runs once at shutdown. */
//void SDL_AppQuit(void* appstate, SDL_AppResult result)
//{
//    if (renderer) SDL_DestroyRenderer(renderer);
//    if (window) SDL_DestroyWindow(window);
//}