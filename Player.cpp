#include "Player.h"
#include "Game.h"
#include "Utils.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

Player::Player()
{
    SetPositionsByMapXY(
    Game::INI.Get<int>("player", "start_x"),
    Game::INI.Get<int>("player", "start_y")
    );
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void Player::HandleInput(const float t_dt, const olc::PixelGameEngine* t_pge)
{
    const auto turnSpeed{ Game::INI.Get<float>("player", "turn_speed") };
    const auto moveSpeed{ Game::INI.Get<float>("player", "move_speed") };

    if (t_pge->GetKey(olc::Key::LEFT).bHeld)
    {
        radians = clamp_radians(radians - t_dt * turnSpeed);
    }
    if (t_pge->GetKey(olc::Key::RIGHT).bHeld)
    {
        radians = clamp_radians(radians + t_dt * turnSpeed);
    }

    // backup screen position
    auto screenX{ m_screenPosition.x };
    auto screenY{ m_screenPosition.y };

    // calc new screen position
    if (t_pge->GetKey(olc::Key::UP).bHeld)
    {
        adjust_screen_position_by_angle(screenX, screenY, radians, t_dt, moveSpeed, true);
    }
    if (t_pge->GetKey(olc::Key::DOWN).bHeld)
    {
        adjust_screen_position_by_angle(screenX, screenY, radians, t_dt, moveSpeed, false);
    }

    // set new player map and screen positions if there is no wall
    if (!Map::IsMapTypeAtScreenPosition(screenX, screenY, Map::WALL))
    {
        SetPositionsByScreenXY(screenX, screenY);
    }
}

void Player::CastRays()
{
    const auto nrOfRays{ Game::INI.Get<int>("player", "nr_of_rays") };
    const auto fovDeg{ Game::INI.Get<float>("player", "fov") };
    const auto fovRad{ fovDeg / 180.0f * static_cast<float>(M_PI) };
    const auto startRad{ radians - fovRad / 2.0f };
    const auto radPerRay{ fovRad / static_cast<float>(nrOfRays) };

    clear_vector(rays);

    for (auto i{ 0 }; i < nrOfRays; ++i)
    {
        const auto radians{ clamp_radians(startRad + static_cast<float>(i) * radPerRay) };
        Ray vRay{ m_screenPosition, radians, Ray::VERTICAL };
        Ray hRay{ m_screenPosition, radians, Ray::HORIZONTAL };
        GetVerticalIntersection(vRay);
        GetHorizontalIntersection(hRay);
        rays.push_back(vRay.length < hRay.length ? vRay : hRay);
    }
}

void Player::RenderPlayer(olc::PixelGameEngine* t_pge, const float t_xOffset, const float t_yOffset, const float t_scale) const
{
    const auto startX{ t_xOffset + m_screenPosition.x * t_scale };
    const auto startY{ t_yOffset + m_screenPosition.y * t_scale };
    const auto startXInt{ static_cast<int>(startX) };
    const auto startYInt{ static_cast<int>(startY) };

    t_pge->FillCircle(startXInt, startYInt, 2, olc::DARK_GREEN);

    const auto lineLength{ Game::INI.Get<float>("player", "line_length") };

    const auto endPositionX{ startX + lineLength * cosf(radians) };
    const auto endPositionY{ startY + lineLength * sinf(radians) };

    t_pge->DrawLine(startXInt, startYInt, static_cast<int>(endPositionX), static_cast<int>(endPositionY), olc::DARK_GREEN);
}

void Player::RenderRays(olc::PixelGameEngine* t_pge, const float t_xOffset, const float t_yOffset, const float t_scale) const
{
    for (const auto& ray : rays)
    {
        ray.Render(t_pge, t_xOffset, t_yOffset, t_scale);
    }
}

void Player::RenderDebugInfo(olc::PixelGameEngine* t_pge) const
{
    t_pge->DrawString(500, 4, "Screen position: " + to_string_with_precision(m_screenPosition.x) + ", " + to_string_with_precision(m_screenPosition.y), olc::WHITE);
    t_pge->DrawString(500, 14, "Map position: " + std::to_string(m_mapPosition.x) + ", " + std::to_string(m_mapPosition.y), olc::WHITE);
    t_pge->DrawString(500, 24, "Angle in rad: " + to_string_with_precision(radians), olc::WHITE);
    t_pge->DrawString(500, 34, "Angle in deg: " + to_string_with_precision(radians * (180.0f / M_PI)), olc::WHITE);
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void Player::SetPositionsByMapXY(const int t_mapX, const int t_mapY)
{
    const auto tileSize{ Game::INI.Get<float>("map", "tile_size") };

    m_screenPosition = map_to_screen(t_mapX, t_mapY, tileSize);
    m_mapPosition.x = t_mapX;
    m_mapPosition.y = t_mapY;
}

void Player::SetPositionsByScreenXY(const float t_screenX, const float t_screenY)
{
    const auto tileSize{ Game::INI.Get<float>("map", "tile_size") };

    m_mapPosition = screen_to_map(t_screenX, t_screenY, tileSize);
    m_screenPosition.x = t_screenX;
    m_screenPosition.y = t_screenY;
}

//-------------------------------------------------
// Rays
//-------------------------------------------------

void Player::GetVerticalIntersection(Ray& t_ray) const
{
    const auto tileSize{ Game::INI.Get<float>("map", "tile_size") };

    const auto tanRayAngle{ tanf(t_ray.radians) };
    const auto facingRight{ is_facing_right(t_ray.radians) };

    /*
             scFirstX   or  scFirstX
                    |       |
                    x-------x
                    |       |
        facing left |   p   | facing right
                    |       |
                    ---------
    */

    const auto screenFirstX{ static_cast<float>(m_mapPosition.x) * tileSize +
        (facingRight ? tileSize : 0.0f)
    };

    /*
    deltaY = deltaX * tan(a)
              + x
            +   |
          +     | deltaY
        + a     |
        ---------
         deltaX
    */

    const auto deltaX{ screenFirstX - m_screenPosition.x };
    const auto deltaY{ deltaX * tanRayAngle };
    const auto screenFirstY{ m_screenPosition.y + deltaY };

    // to increment or decrement the x/y coordinate for each step
    const auto deltaXPerTile{ facingRight ? tileSize : -tileSize };
    const auto deltaYPerTile{ deltaXPerTile * tanRayAngle };

    // initialize variables to iterate through map cells until a wall is found
    auto hitWall{ false };
    auto currentScreenX{ screenFirstX };
    auto currentScreenY{ screenFirstY };

    // loop until the ray hits a wall or goes out of bounds.
    while (!hitWall)
    {
        const auto currentMapPosition{ screen_to_map(currentScreenX, currentScreenY, tileSize) };
        const auto currentMapX{ currentMapPosition.x - (facingRight ? 0 : 1) };
        const auto currentMapY{ currentMapPosition.y };

        if (is_position_not_on_map(currentMapX, currentMapY, Map::MAP_WIDTH, Map::MAP_HEIGHT))
        {
            break;
        }

        hitWall = Map::IsWallTypeAtMapPosition(currentMapX, currentMapY);
        if (!hitWall)
        {
            currentScreenX += deltaXPerTile;
            currentScreenY += deltaYPerTile;
        }

        t_ray.screenHitPosition.x = currentScreenX;
        t_ray.screenHitPosition.y = currentScreenY;

        const auto dx{ currentScreenX - m_screenPosition.x };
        const auto dy{ currentScreenY - m_screenPosition.y };

        t_ray.length = sqrtf(dx * dx + dy * dy);
    }
}

void Player::GetHorizontalIntersection(Ray& t_ray) const
{
    const auto tileSize{ Game::INI.Get<float>("map", "tile_size") };

    const auto tanRayAngle{ tanf(t_ray.radians) };
    const auto facingUp{ is_facing_up(t_ray.radians) };

    const auto screenFirstY{ static_cast<float>(m_mapPosition.y) * tileSize + (facingUp ? 0 : tileSize) };
    const auto screenFirstX{ m_screenPosition.x + (screenFirstY - m_screenPosition.y) / tanRayAngle };

    const auto deltaYPerTile{ facingUp ? -tileSize : tileSize };
    const auto deltaXPerTile{ deltaYPerTile / tanRayAngle };

    auto hitWall{ false };
    auto currentScreenX{ screenFirstX };
    auto currentScreenY{ screenFirstY };

    while (!hitWall)
    {
        const auto currentMapPosition{ screen_to_map(currentScreenX, currentScreenY, tileSize) };
        const auto currentMapX{ currentMapPosition.x };
        const auto currentMapY{ currentMapPosition.y - (facingUp ? 1 : 0) };

        if (is_position_not_on_map(currentMapX, currentMapY, Map::MAP_WIDTH, Map::MAP_HEIGHT))
        {
            break;
        }

        hitWall = Map::IsWallTypeAtMapPosition(currentMapX, currentMapY);
        if (!hitWall)
        {
            currentScreenX += deltaXPerTile;
            currentScreenY += deltaYPerTile;
        }

        t_ray.screenHitPosition.x = currentScreenX;
        t_ray.screenHitPosition.y = currentScreenY;

        const auto dx{ currentScreenX - m_screenPosition.x };
        const auto dy{ currentScreenY - m_screenPosition.y };

        t_ray.length = sqrtf(dx * dx + dy * dy);
    }
}
