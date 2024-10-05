#include "Map.h"
#include "Game.h"
#include "Player.h"
#include "Utils.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

Map::Map()
{
    m_texture = std::make_unique<olc::Sprite>("redbrick.png");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void Map::RenderMiniMap(olc::PixelGameEngine* t_pge, const float t_screenX, const float t_screenY, const float t_scale)
{
    const auto scale{ t_scale * Game::INI.Get<float>("map", "tile_size") };
    const auto scaleInt{ static_cast<int>(scale) };

    for (auto y{ 0 }; y < MAP_HEIGHT; ++y)
    {
        for (auto x{ 0 }; x < MAP_WIDTH; ++x)
        {
            const auto screenX{ t_screenX + static_cast<float>(x) * scale };
            const auto screenY{ t_screenY + static_cast<float>(y) * scale };
            const auto screenXInt{ static_cast<int>(screenX) };
            const auto screenYInt{ static_cast<int>(screenY) };

            if (IsMapTypeAtMapPosition(x, y, EMPTY))
            {
                t_pge->DrawRect(screenXInt, screenYInt, scaleInt, scaleInt, olc::RED);
            }
            else if (IsMapTypeAtMapPosition(x, y, WALL))
            {
                t_pge->FillRect(screenXInt, screenYInt, scaleInt, scaleInt, olc::WHITE);
                t_pge->DrawRect(screenXInt, screenYInt, scaleInt, scaleInt, olc::RED);
            }
        }
    }
}

void Map::Render(olc::PixelGameEngine* t_pge, const Player* t_player) const
{
    const auto projectionPlane{ Game::INI.Get<float>("map", "projection_plane") };
    const auto screenWidth{ Game::INI.Get<int>("window", "width") };
    const auto screenHeight{ Game::INI.Get<float>("window", "height") };
    const auto screenHalfHeight{ screenHeight / 2.0f };
    const auto screenHeightInt{ static_cast<int>(screenHeight) };

    auto x{ 0 };
    const auto rayLineWidth{ screenWidth / Game::INI.Get<int>("player", "nr_of_rays") };

    for (const auto& ray : t_player->rays)
    {
        const auto rayLength{ ray.length * cosf(ray.radians - t_player->radians) };
        const auto wallHeight{ (projectionPlane / rayLength) * screenHeight };
        const auto wallTop{ screenHalfHeight - wallHeight / 2.0f };
        const auto wallBottom{ wallTop + wallHeight };

        // wall
        auto texture{ true };
        if (texture)
        {
            RenderTexturedWall(
                t_pge,
                &ray,
                static_cast<int>(wallTop),
                static_cast<int>(wallBottom),
                wallHeight,
                x,
                rayLineWidth
            );
        }
        else
        {
            RenderPart(
                t_pge,
                x, static_cast<int>(wallTop),
                rayLineWidth, static_cast<int>(wallHeight),
                ray.type == Ray::VERTICAL ? olc::DARK_BLUE : olc::BLUE
            );
        }

        // bottom
        RenderPart(
            t_pge,
            x, static_cast<int>(wallBottom),
            rayLineWidth, screenHeightInt - static_cast<int>(wallBottom),
            olc::DARK_GREY
        );

        // top
        RenderPart(
            t_pge,
            x, 0,
            rayLineWidth, screenHeightInt - static_cast<int>(wallBottom),
            olc::GREY
        );

        x += rayLineWidth;
    }
}

//-------------------------------------------------
// Map value checks
//-------------------------------------------------

bool Map::IsMapTypeAtMapPosition(const int t_mapX, const int t_mapY, const MapType t_mapType)
{
    return MAP.at(calc_map_index(t_mapX, t_mapY, MAP_WIDTH)) == t_mapType;
}

bool Map::IsMapTypeAtScreenPosition(const float t_screenX, const float t_screenY, const MapType t_mapType)
{
    const auto mapPosition{ screen_to_map(t_screenX, t_screenY, Game::INI.Get<float>("map", "tile_size")) };
    return IsMapTypeAtMapPosition(mapPosition.x, mapPosition.y, t_mapType);
}

bool Map::IsWallTypeAtMapPosition(const int t_mapX, const int t_mapY)
{
    return MAP.at(calc_map_index(t_mapX, t_mapY, MAP_WIDTH)) == WALL;
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void Map::RenderTexturedWall(
    olc::PixelGameEngine* t_pge,
    const Ray* t_ray,
    const int t_wallTop,
    const int t_wallBottom,
    const float t_wallHeight,
    const int t_x,
    const int t_rayLineWidth
) const
{

    auto shadow{ 1.0f };

    int textureX = static_cast<int>(t_ray->screenHitPosition.x) % 64;
    if (t_ray->type == Ray::VERTICAL) {
        shadow = 0.7f;
        textureX = static_cast<int>(t_ray->screenHitPosition.y) % 64;
    }

    for (auto y{ t_wallTop }; y < t_wallBottom; ++y)
    {
        const auto yf{ static_cast<float>(y) };
        const auto wtf{ static_cast<float>(t_wallTop) };

        auto textureY{ static_cast<int>((yf - wtf) / t_wallHeight * 64) };

        if (textureY < 0)
        {
            textureY = 0;
        }

        if (textureY >= 64)
        {
            textureY = 63;
        }

        t_pge->FillRect(
            t_x, y,
            t_rayLineWidth, 1,
            m_texture->GetPixel(textureX, textureY) * shadow
        );
    }
}

void Map::RenderPart(
    olc::PixelGameEngine* t_pge,
    const int t_startX, const int t_startY,
    const int t_width, const int t_height,
    const olc::Pixel t_color
)
{
    t_pge->FillRect(t_startX, t_startY, t_width, t_height, t_color);
}
