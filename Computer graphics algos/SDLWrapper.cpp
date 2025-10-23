#include "SDLWrapper.h"



SDLWrapper::SDLWrapper(int width, int height)
	:width(width), height(height)
{

}

SDL_AppResult SDLWrapper::init()
{
    if (SDL_CreateWindowAndRenderer("A terrific window title", width, height,
        0, &pWindow, &pRenderer) != 0)
    {
        SDL_Log("Failed: %s", SDL_GetError()); 
        return SDL_APP_FAILURE;
    }

	return SDL_APP_CONTINUE;
}

SDLWrapper::~SDLWrapper()
{
    quit(SDL_APP_SUCCESS); 

}

int SDLWrapper::run(const Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts,
    const std::vector<Color>& colors)
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
            if (handleEvent(&event, camera) == SDL_APP_SUCCESS)
                quitFlag = true;
        }

        if (iterate(cube, camera, localVerts, colors) == SDL_APP_FAILURE)
            quitFlag = true;
    }

    quit(SDL_APP_SUCCESS);
    SDL_Quit();
    return 0;
}



SDL_AppResult SDLWrapper::iterate(const Cube& cube, Camera& camera, const std::vector<Vec4>& localVerts, 
    const std::vector<Color>& colors)
{

    auto frameStart = std::chrono::high_resolution_clock::now();

    /*Set the transformations to be applied to object*/
    Vec4 rot(M_PI / 6.0f * frameCount,
        M_PI / 5.0f /** frameCount*/
        , M_PI / 4.0f/* * frameCount*/, 0.0f); 
    Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f); 
    Vec4 trans(0.0f, 0.0f, -3.0f, 1.0f); 

    /*Apply the transformations and get screen-equivalent coordinates*/
    Mesh mesh(localVerts, trans, rot, scale);
    std::vector<Vec4> worldCubeVerts = mesh.applyModelMatrix();
    auto screenSpaceCubeVerts = camera.projectToScreen(worldCubeVerts, width, height);

    /*Fill the faces of the object OR draw in wireframe mode*/
    auto rasteredPixels = cube.filledRasterize(screenSpaceCubeVerts, colors);
    
    draw(rasteredPixels);

    advanceFrame(frameStart); 

    return SDL_APP_CONTINUE; //what is a fail condition in this function? 
}

void SDLWrapper::draw(const std::unordered_map<Vec2, Color>& rasteredPixels) const
{
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

}

void SDLWrapper::advanceFrame(const std::chrono::steady_clock::time_point& frameStart)
{
    auto frameEnd = std::chrono::high_resolution_clock::now();
    long long frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
    /*Unlikely to need a long long, but .count() returns long long*/
    if (frameTime < frameDelay.count())
        std::this_thread::sleep_for(frameDelay - std::chrono::milliseconds(frameTime));

    frameCount++;
}


SDL_AppResult SDLWrapper::handleEvent(SDL_Event* pEvent, Camera& camera)
{
    if (pEvent->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    else if (pEvent->type == SDL_EVENT_KEY_DOWN)
    {
        Uint32 keyID = pEvent->key.key;

        std::cout << keyID << "\n";

        float stepSize = 0.1f;

        switch (keyID)
        {
        case SDLK_P:
            std::cout << "Pausing animation...\n";
            std::system("pause");
            break; 

        case SDLK_W:
            std::cout << "Moving camera forward\n";   
            camera.moveForward(stepSize);
            break;

        case SDLK_S: 
            std::cout << "Moving backerds\n";
            camera.moveBackward(stepSize);
            break; 

        default: 
            std::cout << "Unhandled key pressed\n"; 
            break; 
        }

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




