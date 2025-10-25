#include "SDLWrapper.h"

#include<chrono> 
#include<thread> 


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

int SDLWrapper::run(const Mesh& mesh, MeshInstance& meshInstance, 
    Camera& camera, const std::vector<Vec4>& localVerts,
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
            if (handleEvent(&event, camera, meshInstance) == SDL_APP_SUCCESS)
                quitFlag = true;
        }

        if (iterate(mesh, meshInstance, camera, localVerts, colors) == SDL_APP_FAILURE)
            quitFlag = true;
    }

    quit(SDL_APP_SUCCESS);
    SDL_Quit();
    return 0;
}



SDL_AppResult SDLWrapper::iterate(const Mesh& mesh, MeshInstance& meshInstance, 
    Camera& camera, const std::vector<Vec4>& localVerts, 
    const std::vector<Color>& colors)
{

    auto frameStart = std::chrono::high_resolution_clock::now();

    std::vector<Vec4> worldVerts; 

    meshInstance.applyTransformation(localVerts, worldVerts);
    //auto screenVerts = camera.projectToScreen(worldVerts, mesh);


    /*In the functions below, I am returning potentially-large vectors<Vec4> - modify to void in the future*/

    auto viewVerts = camera.applyView(worldVerts); 
    //cull -> pass "surviving" triangle face indices to Rasterizer below
    auto frontFaceIndices = camera.getFrontFaceIndices(viewVerts, mesh); 
    
    /*Apply remaining 3 operations to project to screen space*/
    auto projectedVerts = camera.applyProjection(viewVerts);
    auto ndcVerts = camera.applyPerspectiveDivide(projectedVerts);
    auto screenVerts = camera.ndcToScreen(ndcVerts);

    /*Fill the faces of the object OR draw in wireframe mode*/
    //auto rasterizedPixels = Rasterizer::getFilledFaces(mesh, screenVerts, colors, width, height);

    auto rasterizedPixels = Rasterizer::getFilledFaces(frontFaceIndices, screenVerts, colors, width, height);

    draw(rasterizedPixels);

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


SDL_AppResult SDLWrapper::handleEvent(SDL_Event* pEvent, Camera& camera, MeshInstance& meshInstance)
{
    if (pEvent->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    else if (pEvent->type == SDL_EVENT_KEY_DOWN)
    {
        Uint32 keyID = pEvent->key.key;

        std::cout << keyID << "\n";

        float cameraStepSize = 0.1f; 
        bool isPaused = false; 
        float thetaRotation = M_PI / 10.0f; //let this be the same for x, y, and z
        float objectStepSize = 0.1f; 

        /*Might consider moving this to a helper method (ex: handleKeyboardEvent)*/
        switch (keyID) //note that cases are organized alphabetically (not "logically") 
        {
        case SDLK_A: //note that the user is allowed to enter 'a' here 
            std::cout << "Moving left " << cameraStepSize << "\n";
            camera.moveLeft(cameraStepSize); 
            break;

        case SDLK_D: //the K stands for "key" probably
            std::cout << "Moving right " << cameraStepSize << "\n";
            camera.moveRight(cameraStepSize);
            break;

        case SDLK_DOWN:
            std::cout << "Moving down " << cameraStepSize << "\n";
            camera.moveDown(cameraStepSize);
            break;

        case SDLK_P:
            std::cout << "Pausing animation...\n";
            std::system("pause"); 
            break; 

        case SDLK_S: 
            std::cout << "Moving backerds\n";
            camera.moveBackward(cameraStepSize);
            break; 

        case SDLK_T: //added this case just to test getting object off of the screen - I should improve this later
                    //or just give a Mesh its own "natural" movement/animation that is not controlled by the user
            std::cout << "Translating object to the right\n";
            auto newTranslation = meshInstance.getTranslation(); 
            newTranslation.x -= objectStepSize;
            meshInstance.setTranslation(newTranslation); 
            break; 

        case SDLK_UP:
            std::cout << "Moving up " << cameraStepSize << "\n";
            camera.moveUp(cameraStepSize);
            break; 

        case SDLK_W:
            std::cout << "Moving camera forward\n";
            camera.moveForward(cameraStepSize);
            break;
  
        case SDLK_X: 
            std::cout << "Rotating about x by " << ((180.0f) / M_PI) * thetaRotation << "\n";
            auto newXRotation = meshInstance.getRotation(); //get the current rotation vector 
            newXRotation.x += thetaRotation; //and update the x-component
            meshInstance.setRotation(newXRotation);
            break;

        case SDLK_Y:
            std::cout << "Rotating about y by " << ((180.0f) / M_PI) * thetaRotation << "\n";
            auto newYRotation = meshInstance.getRotation();
            newYRotation.y += thetaRotation;
            meshInstance.setRotation(newYRotation);
            break;
            
        case SDLK_Z:
            std::cout << "Rotating about zed by " << ((180.0f) / M_PI) * thetaRotation << "\n";
            auto newZRotation = meshInstance.getRotation();
            newZRotation.z += thetaRotation; 
            meshInstance.setRotation(newZRotation);
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




