#include "SDLWrapper.h"



SDLWrapper::SDLWrapper(int width, int height)
	:width(width), height(height)
{
    //localCubeVerts = cube.getLocalCubeVerts(); 

    //camera = Camera(fovY);
}

SDLWrapper::~SDLWrapper()
{
    quit(SDL_APP_SUCCESS); //MIGHT be normal, I suppose

}

int SDLWrapper::run(Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts)
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    if (!init()) {
        SDL_Quit();
        return -1;
    }

    bool quitFlag = false;
    SDL_Event event;

    while (!quitFlag)
    {
        // Poll events
        while (SDL_PollEvent(&event))
        {
            if (handleEvent(&event) == SDL_APP_SUCCESS)
                quitFlag = true;
        }

        // Call iterate
        if (iterate(cube, camera, localVerts) == SDL_APP_FAILURE)
            quitFlag = true;
    }

    quit(SDL_APP_SUCCESS);
    SDL_Quit();
    return 0;
}




SDL_AppResult SDLWrapper::init()
{
    if (SDL_CreateWindowAndRenderer("A terrific window title", width, height,
        0, &pWindow, &pRenderer) != 0)
    {
        SDL_Log("Failed: %s", SDL_GetError()); //SDL_Log is NOT writing to cout ... 
        return SDL_APP_FAILURE;
    }

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDLWrapper::iterate(Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts)
{
    try
    {
        auto frameStart = std::chrono::high_resolution_clock::now();


        Vec4 rot(M_PI / 6.0f * frameCount, M_PI / 5.0f * frameCount, M_PI / 4.0f * frameCount, 0.0f); 
        Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f); 
        Vec4 trans(0.0f, 0.0f, -3.0f, 1.0f); 

        Mesh mesh(localVerts, trans, rot, scale);

        std::vector<Vec4> worldCubeVerts = mesh.applyModelMatrix();


        auto screenSpaceCubeVerts = camera.projectToScreen(worldCubeVerts, width, height);

        std::vector<Color> colors;
        auto colorEnums = Color::getBroadColorPalette();
        for (const auto& current : colorEnums)
        {
            colors.push_back(Color(current));
        }

        auto rasteredPixels = cube.filledRasterize(screenSpaceCubeVerts, colors);

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        for (auto& [pos, color] : rasteredPixels)
        {
            Uint8 r, g, b;
            color.getRGB8(r, g, b);
            SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);

            SDL_RenderPoint(pRenderer, pos.x, pos.y);
        }
        SDL_RenderPresent(pRenderer);
        std::cout << "Draw it, draw it real good- frame count: " << frameCount << "\n";

        auto frameEnd = std::chrono::high_resolution_clock::now();
        long long frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
        /*Unlikely to need a long long, but .count() returns long long*/
        if (frameTime < FRAME_DELAY.count())
            std::this_thread::sleep_for(FRAME_DELAY - std::chrono::milliseconds(frameTime));

        frameCount++;

        return SDL_APP_CONTINUE;
    }

    catch (const MyException& e)
    {
        std::cout << e.whatWentWrong();
        return SDL_APP_FAILURE;
    }

}

SDL_AppResult SDLWrapper::handleEvent(SDL_Event* pEvent)
{
    if (pEvent->type == SDL_EVENT_KEY_DOWN || pEvent->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE; 
}

void SDLWrapper::quit(SDL_AppResult)
{
    if (pRenderer)
    {
        SDL_DestroyRenderer(pRenderer);
    }

    if (pWindow)
    {
        SDL_DestroyWindow(pWindow);
    }
}


