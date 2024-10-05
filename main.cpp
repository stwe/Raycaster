#define OLC_PGE_APPLICATION
#include "Game.h"
#include "Log.h"

int main()
{
    Log::Init();

    FPS_LOG_DEBUG("[main()] Starting main.");
    FPS_LOG_DEBUG("[main()] Logger was initialized.");

    if (Game game; game.Construct(
        Game::INI.Get<int>("window", "width"),
        Game::INI.Get<int>("window", "height") ,
        1, 1)
    )
    {
        game.Start();
    }

    return 0;
}
