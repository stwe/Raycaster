#include "Game.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

Game::Game()
{
    sAppName = "First Person Shooter";
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool Game::OnUserCreate()
{
    return true;
}

bool Game::OnUserUpdate(const float t_dt)
{
    if (GetKey(olc::Key::ESCAPE).bPressed)
    {
        return false;
    }

    Clear(olc::BLACK);

    m_player.HandleInput(t_dt, this);
    m_map.Render(this, &m_player);

    const auto screenWidth{ INI.Get<float>("window", "width") };
    const auto screenHeight{ INI.Get<float>("window", "height") };
    const auto tileSize{ INI.Get<float>("map", "tile_size") };
    const auto scale{ INI.Get<float>("mini_map", "scale") };

    const auto xOffset{ screenWidth - Map::MAP_WIDTH * tileSize * scale };
    const auto yOffset{ screenHeight - Map::MAP_HEIGHT * tileSize * scale };
    Map::RenderMiniMap(this, xOffset, yOffset ,scale);
    m_player.RenderPlayer(this, xOffset, yOffset ,scale);
    m_player.CastRays();
    m_player.RenderRays(this, xOffset, yOffset ,scale);
    m_player.RenderDebugInfo(this);

    return true;
}
