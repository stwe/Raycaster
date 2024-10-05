#pragma once

#include "olcPixelGameEngine.h"

class Ray
{
public:
    //-------------------------------------------------
    // Types
    //-------------------------------------------------

    /**
     * @brief Enumeration representing the type of the ray.
     */
    enum RayType
    {
        VERTICAL, HORIZONTAL
    };

    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    /**
     * @brief The screen position where the ray is cast from.
     */
    olc::vf2d screenPosition{ 0.0f, 0.0f };

    /**
     * @brief The length of the ray, initialized to the maximum float value.
     */
    float length{ std::numeric_limits<float>::max() };

    /**
     * @brief The angle of the ray in radians.
     */
    float radians{ 0.0f };

    /**
     * @brief The position on the screen where the ray hits an object.
     */
    olc::vf2d screenHitPosition{ 0.0f, 0.0f };

    /**
     * @brief The type of the ray (either vertical or horizontal).
     */
    RayType type;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    Ray() = delete;

    /**
     * @brief Constructs a new Ray object.
     *
     * @param t_screenPosition The starting position of the ray on the screen.
     * @param t_radians The angle of the ray in radians.
     * @param t_type The type of the ray (vertical or horizontal).
     */
    Ray(const olc::vf2d& t_screenPosition, float t_radians, RayType t_type);

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    /**
     * @brief Renders the ray on the screen.
     *
     * @param t_pge Pointer to the PixelGameEngine object.
     * @param t_xOffset The x-offset.
     * @param t_yOffset The y-offset.
     * @param t_scale The scale factor.
     */
    void Render(olc::PixelGameEngine* t_pge, float t_xOffset, float t_yOffset, float t_scale) const;

protected:

private:

};
