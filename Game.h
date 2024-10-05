#pragma once

#include "ini.h"
#include "Player.h"
#include "Map.h"

//-------------------------------------------------
// Game
//-------------------------------------------------

class Game : public olc::PixelGameEngine
{
public:
    //-------------------------------------------------
    // Constants
    //-------------------------------------------------

    /**
     * @brief The name of the Ini-File to read.
     */
    inline static const inih::INIReader INI{ "./config.ini" };

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    Game();

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool OnUserCreate() override;
    bool OnUserUpdate(float t_dt) override;

protected:

private:
    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    Player m_player;
    Map m_map;
};
