#ifndef GOBANG_GAME_H
#define GOBANG_GAME_H

#include <SDL2/SDL.h>
#include <vector>

class AbstractObject
{
public:
    virtual bool draw(SDL_Renderer* renderer)
    {
        return true;
    }

    virtual bool update(SDL_Event* event)
    {
        return true;
    }
};

class Game
{
public:
    Game(int width=640, int height=400, const char* title="Game");
    int execute(int argc, char* argv[]);
    int addObject(AbstractObject* obj);

    int mainloop();
    void draw(SDL_Renderer* renderer);
    void update(SDL_Event* event);

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    int _fps;
    bool _running;
    std::vector<AbstractObject*> _objects;

    void default_draw(SDL_Renderer* renderer);
    void default_update(SDL_Event* event);

    SDL_sem* _fpsSem;
    static Uint32 fpsTimerCallback(Uint32 interval, void* args)
    {
        SDL_sem* fpsSem = ((Game*)args)->_fpsSem;
        if(0 != SDL_SemPost(fpsSem))
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SemPost : %s", SDL_GetError());
        }

        return 0;
    }
};

#endif