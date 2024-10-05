#pragma once

#include "olcPixelGameEngine.h"

//-------------------------------------------------
// Linear index
//-------------------------------------------------

/**
 * @brief Calculates the linear index of a 2D position on a map.
 *
 * @param t_x The x-coordinate on the map.
 * @param t_y The y-coordinate on the map.
 * @param t_mapWidth The width of the map.
 *
 * @return The linear index corresponding to the position on the map.
 */
[[nodiscard]] inline int calc_map_index(
    const int t_x, const int t_y,
    const int t_mapWidth
)
{
    return t_y * t_mapWidth + t_x;
}

//-------------------------------------------------
// Map position
//-------------------------------------------------

/**
 * @brief Checks if a map position is not within the bounds of the map.
 *
 * @param t_x The x-coordinate on the map.
 * @param t_y The y-coordinate on the map.
 * @param t_mapWidth The width of the map.
 * @param t_mapHeight The height of the map.
 *
 * @return True if the position is outside the bounds of the map, otherwise false.
 */
[[nodiscard]] inline bool is_position_not_on_map(
    const int t_x, const int t_y,
    const int t_mapWidth, const int t_mapHeight
)
{
    return t_x >= t_mapWidth || t_y >= t_mapHeight || t_x < 0 || t_y < 0;
}

/**
 * @brief Checks if a map position is within the bounds of the map.
 *
 * @param t_x The x-coordinate on the map.
 * @param t_y The y-coordinate on the map.
 * @param t_mapWidth The width of the map.
 * @param t_mapHeight The height of the map.
 *
 * @return True if the position is within the bounds of the map, otherwise false.
 */
[[nodiscard]] inline bool is_position_on_map(
    const int t_x, const int t_y,
    const int t_mapWidth, const int t_mapHeight
)
{
    return t_x < t_mapWidth && t_y < t_mapHeight && t_x >= 0 && t_y >= 0;
}

/**
 * @brief Converts screen coordinates to map coordinates.
 *
 * @param t_screenX The x-coordinate on the screen.
 * @param t_screenY The y-coordinate on the screen.
 * @param t_tileSize The size of each tile.
 *
 * @return
 */
[[nodiscard]] inline olc::vi2d screen_to_map(const float t_screenX, const float t_screenY, const float t_tileSize)
{
    return { static_cast<int>(t_screenX / t_tileSize), static_cast<int>(t_screenY / t_tileSize) };
}

/**
 * @brief Converts map coordinates to screen coordinates.
 *
 * @param t_x The x-coordinate on the map.
 * @param t_y The y-coordinate on the map.
 * @param t_tileSize The size of each tile.
 *
 * @return The screen coordinates of the center of the tile.
 */
[[nodiscard]] inline olc::vf2d map_to_screen(const int t_x, const int t_y, const float t_tileSize)
{
    const auto halfTileSize{ (t_tileSize / 2.0f) };
    return { static_cast<float>(t_x) * t_tileSize + halfTileSize, static_cast<float>(t_y) * t_tileSize + halfTileSize };
}

/**
 * @brief Updates the screen position based on the given angle and movement direction.
 *
 * @param t_screenX Reference to the x-coordinate of the screen position.
 * @param t_screenY Reference to the y-coordinate of the screen position.
 * @param t_radians The angle in radians indicating the direction of movement.
 * @param t_dt Delta time, representing the time elapsed since the last update.
 * @param t_moveSpeed The speed at which the screen position should move.
 * @param t_moveForward Boolean flag indicating the direction of movement: `true` for forward, `false` for backward.
 */
inline void adjust_screen_position_by_angle(
    float& t_screenX, float& t_screenY,
    const float t_radians,
    const float t_dt,
    const float t_moveSpeed,
    const bool t_moveForward
)
{
    const auto direction{ t_moveForward ? 1.0f : -1.0f };
    t_screenX += direction * cosf(t_radians) * t_dt * t_moveSpeed;
    t_screenY += direction * sinf(t_radians) * t_dt * t_moveSpeed;
}

//-------------------------------------------------
// Angle
//-------------------------------------------------

/**
 * @brief Clamps an angle in radians to the range [0, 2π).
 *
 * @param t_radians The angle in radians to be clamped.
 *
 * @return The normalized angle in radians within the range [0, 2π).
 */
[[nodiscard]] inline float clamp_radians(const float t_radians)
{
    constexpr float twoPi{ 2.0f * M_PI };
    auto normalized{ std::fmod(t_radians, twoPi) };
    if (normalized < 0)
    {
        normalized += twoPi;
    }

    return normalized;
}

/**
 * @brief Determines if the direction represented by the given angle in radians is facing right.
 *
 * @param t_radians The angle in radians to evaluate.
 *
 * @return True if the direction is facing right, false otherwise.
 */
[[nodiscard]] inline bool is_facing_right(const float t_radians)
{
    return t_radians <= 0.5f * M_PI || t_radians >= 1.5f * M_PI;
}

/**
 * @brief Determines if the direction represented by the given angle in radians is facing up.
 *
 * @param t_radians The angle in radians to evaluate.
 *
 * @return True if the direction is facing up, false otherwise.
 */
[[nodiscard]] inline bool is_facing_up(const float t_radians)
{
    return t_radians >= M_PI;
}

//-------------------------------------------------
// Arrays
//-------------------------------------------------

/**
 * @brief Clears a given vector by swapping it with an empty one.
 *
 * This function releases the memory used by a vector by swapping it with an empty vector.
 * After calling this function, the original vector will be empty, and its memory will be freed.
 *
 * @tparam T The type of elements stored in the vector.
 * @param t_vec The vector to be cleared.
 */
template <typename T>
void clear_vector(std::vector<T>& t_vec)
{
    std::vector<T>().swap(t_vec);
}

//-------------------------------------------------
// Strings
//-------------------------------------------------

/**
 * @brief Converts a floating point value to a string with a specified number of decimal places.
 *
 * @tparam T The floating point type (float, double).
 * @param t_value The floating point value to be converted.
 * @param t_precision The number of decimal places to include in the string. Default is 2.
 *
 * @return std::string A string representation of the floating point value with the specified precision.
 */
template<typename T>
[[nodiscard]] std::string to_string_with_precision(const T t_value, const int t_precision = 2)
{
    static_assert(std::is_floating_point<T>::value, "Template argument must be a floating point type.");
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(t_precision) << t_value;
    return stream.str();
}
