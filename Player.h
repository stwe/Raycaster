#pragma once

#include "Ray.h"

//-------------------------------------------------
// Player
//-------------------------------------------------

/**
 * @brief Class representing the player.
 */
class Player
{
public:
    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    /**
     * @brief The player's rotation in radians.
     */
    float radians{ 0.0f };

    /**
     * @brief A collection of rays representing the player's field of view.
     */
    std::vector<Ray> rays;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    Player();

    Player(const Player& t_other) = delete;
    Player(Player&& t_other) noexcept = delete;
    Player& operator=(const Player& t_other) = delete;
    Player& operator=(Player&& t_other) noexcept = delete;

    ~Player() noexcept = default;

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    /**
     * @brief Handles player input for movement and rotation.
     *
     * @param t_dt The delta time since the last frame, used to adjust movement speed.
     * @param t_pge Pointer to the PixelGameEngine object.
     */
    void HandleInput(float t_dt, const olc::PixelGameEngine* t_pge);

    /**
     * @brief Casts rays to detect walls and other objects in the player's field of view.
     */
    void CastRays();

    /**
     * @brief Renders the player.
     *
     * This method draws the player on the screen based on its current position,
     * offset, and scale. It visually represents the player in the game world.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     * @param t_xOffset The x-offset to apply to the player's position.
     * @param t_yOffset The y-offset to apply to the player's position.
     * @param t_scale The scale factor for rendering the player.
     */
    void RenderPlayer(olc::PixelGameEngine* t_pge, float t_xOffset, float t_yOffset, float t_scale) const;

    /**
     * @brief Renders the rays cast from the player's position.
     *
     * This method visually represents the rays in the game world, allowing the
     * player to see the direction and length of each ray as they detect walls
     * and objects.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     * @param t_xOffset The x-offset.
     * @param t_yOffset The y-offset.
     * @param t_scale The scale factor.
     */
    void RenderRays(olc::PixelGameEngine* t_pge, float t_xOffset, float t_yOffset, float t_scale) const;

    /**
     * @brief Renders debug information.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     */
    void RenderDebugInfo(olc::PixelGameEngine* t_pge) const;

    //-------------------------------------------------
    // Setter
    //-------------------------------------------------

    /**
     * @brief Set positions based on the provided map coordinates.
     *
     * @param t_mapX The x-coordinate on the map.
     * @param t_mapY The y-coordinate on the map.
     */
    void SetPositionsByMapXY(int t_mapX, int t_mapY);

    /**
     * @brief Set positions based on the provided screen coordinates.
     *
     * @param t_screenX The x-coordinate on the screen.
     * @param t_screenY The y-coordinate on the screen.
     */
    void SetPositionsByScreenXY(float t_screenX, float t_screenY);

    //-------------------------------------------------
    // Rays
    //-------------------------------------------------

    /**
     * @brief Calculate the vertical intersection of a ray.
     *
     * @param t_ray Reference to the ray for which to compute the intersection.
     */
    void GetVerticalIntersection(Ray& t_ray) const;

    /**
     * @brief Calculate the horizontal intersection of a ray.
     *
     * @param t_ray Reference to the ray for which to compute the intersection.
     */
    void GetHorizontalIntersection(Ray& t_ray) const;

protected:

private:
    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    /**
     * @brief The screen position of the player.
     */
    olc::vf2d m_screenPosition{ 0.0f, 0.0f };

    /**
     * @brief The map position of the player.
     */
    olc::vi2d m_mapPosition{ 0, 0 };
};
