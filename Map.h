#pragma once

#include "olcPixelGameEngine.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

class Player;
class Ray;

//-------------------------------------------------
// Map
//-------------------------------------------------

/**
 * @brief Class representing the game map.
 */
class Map
{
public:
    //-------------------------------------------------
    // Types
    //-------------------------------------------------

    /**
     * @brief Enum representing the different types of tiles in the map.
     */
    enum MapType
    {
        EMPTY, WALL
    };

    //-------------------------------------------------
    // Constants
    //-------------------------------------------------

    /**
     * @brief The width of the map (number of tiles).
     */
    static constexpr auto MAP_WIDTH{ 8 };

    /**
     * @brief The height of the map (number of tiles).
     */
    static constexpr auto MAP_HEIGHT{ 8 };

    /**
     * @brief Array representing the map layout.
     */
    static constexpr std::array<int, MAP_WIDTH * MAP_HEIGHT> MAP{
        WALL,  WALL,  WALL,  WALL,   WALL,  WALL,  WALL, WALL,
        WALL, EMPTY,  WALL, EMPTY,  EMPTY, EMPTY, EMPTY, WALL,
        WALL, EMPTY,  WALL,  WALL,   WALL, EMPTY, EMPTY, WALL,
        WALL, EMPTY, EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, WALL,
        WALL, EMPTY, EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, WALL,
        WALL, EMPTY, EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, WALL,
        WALL, EMPTY, EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, WALL,
        WALL,  WALL,  WALL,  WALL,   WALL,  WALL,  WALL, WALL,
    };

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    Map();

    Map(const Map& t_other) = delete;
    Map(Map&& t_other) noexcept = delete;
    Map& operator=(const Map& t_other) = delete;
    Map& operator=(Map&& t_other) noexcept = delete;

    ~Map() noexcept = default;

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    /**
     * @brief Renders the mini-map on the screen.
     *
     * This method draws a small top-down view of the map, showing the walls, empty spaces,
     * and possibly the player's position, depending on how the map and player are represented.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     * @param t_screenX The x-coordinate on the screen where the mini-map should be drawn.
     * @param t_screenY The y-coordinate on the screen where the mini-map should be drawn.
     * @param t_scale The scaling factor for the mini-map. Values greater than 1.0 will enlarge the map,
     *                while values less than 1.0 will shrink it.
     */
    static void RenderMiniMap(olc::PixelGameEngine* t_pge, float t_screenX, float t_screenY, float t_scale);

    /**
     * @brief Renders the 3D view of the map based on the player's position and perspective.
     *
     * This method projects the 2D map into a 3D view from the player's point of view.
     * It uses raycasting to determine the distance to walls and renders them with appropriate
     * height and texture mapping, simulating a first-person perspective.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     * @param t_player Pointer to the player object.
     */
    void Render(olc::PixelGameEngine* t_pge, const Player* t_player) const;

    //-------------------------------------------------
    // Map value checks
    //-------------------------------------------------

    /**
     * @brief Checks if a specific MapType exists at the given map coordinates.
     *
     * @param t_mapX The x-coordinate on the map.
     * @param t_mapY The y-coordinate on the map.
     * @param t_mapType Type of tile to check for.
     *
     * @return True if the specified MapType is present at the given coordinates, otherwise false.
     */
    [[nodiscard]] static bool IsMapTypeAtMapPosition(int t_mapX, int t_mapY, MapType t_mapType);

    /**
     * @brief Checks if a specific MapType exists at the given screen coordinates.
     *
     * @param t_screenX The x-coordinate on the screen.
     * @param t_screenY The y-coordinate on the screen.
     * @param t_mapType Type of tile to check for.
     *
     * @return True if the specified MapType is present at the given screen coordinates, otherwise false.
     */
    [[nodiscard]] static bool IsMapTypeAtScreenPosition(float t_screenX, float t_screenY, MapType t_mapType);

    /**
     * @brief Checks if a wall exists at the given map coordinates.
     *
     * @param t_mapX The x-coordinate on the map.
     * @param t_mapY The y-coordinate on the map.
     *
     * @return True if there is a wall at the specified coordinates, otherwise false.
     */
    [[nodiscard]] static bool IsWallTypeAtMapPosition(int t_mapX, int t_mapY);

protected:

private:
    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    /**
     * @brief A texture used for rendering walls hit by rays.
     */
    std::unique_ptr<olc::Sprite> m_texture;

    //-------------------------------------------------
    // Render
    //-------------------------------------------------

    void RenderTexturedWall(
        olc::PixelGameEngine* t_pge,
        const Ray* t_ray,
        int t_wallTop,
        int t_wallBottom,
        float t_wallHeight,
        int t_x,
        int t_rayLineWidth
    ) const;

    static void RenderPart(
        olc::PixelGameEngine* t_pge,
        int t_startX, int t_startY,
        int t_width, int t_height,
        olc::Pixel t_color
    );
};
