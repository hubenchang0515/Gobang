#include "game.h"
#include <stdexcept>

Game::Game(int width, int height, const char* title)
{
    /* Init SDL2 */
    if(0 != SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init : %s", SDL_GetError());
        throw std::runtime_error("SDL_Init Error");
    }

    /* Create Window */
    this->_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    width, height, SDL_WINDOW_SHOWN);
    if(this->_window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow : %s", SDL_GetError());
        throw std::runtime_error("SDL_CreateWindow Error");
    }

    /* Create Renderer */
    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
    if(this->_renderer == nullptr)
    {
        this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_SOFTWARE);
        if(this->_renderer == nullptr)
        {
            SDL_DestroyWindow(this->_window);
            SDL_Quit();
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer : %s", SDL_GetError());
            throw std::runtime_error("SDL_CreateRenderer Error");
        }
    }

    /* Init other */
    this->_fps = 60;
    this->_running = true;
    this->_fpsSem = SDL_CreateSemaphore(0);
}

int Game::execute(int argc, char* argv[])
{
    return this->mainloop();
}

int Game::addObject(AbstractObject* obj)
{
    this->_objects.push_back(obj);
    return this->_objects.size();
}

int Game::mainloop()
{
    SDL_Event event;
    while(_running)
    {
        SDL_TimerID fpsTimer = SDL_AddTimer(1000/this->_fps, fpsTimerCallback, this); 
        if(fpsTimer == 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_AddTimer : %s", SDL_GetError());
            throw std::runtime_error("SDL_AddTimer Error");
        }


        this->draw(this->_renderer);
        while(SDL_PollEvent(&event) > 0)
        {
            this->update(&event);
        }


        SDL_SemWait(this->_fpsSem); 
        SDL_RemoveTimer(fpsTimer); 
    }
    
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
    return 0;
}


void Game::draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    for(AbstractObject* obj : this->_objects)
    {
        obj->draw(renderer);
    }
    this->default_draw(renderer);
    SDL_RenderPresent(renderer);
}


void Game::update(SDL_Event* event)
{
    for(AbstractObject* obj : this->_objects)
    {
        if(obj->update(event) == false)
        {
            return;
        }
    }
    this->default_update(event);
}

void Game::default_draw(SDL_Renderer* renderer)
{

}

void Game::default_update(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        this->_running = false;
    }
}