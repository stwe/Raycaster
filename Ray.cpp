#include "Ray.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

Ray::Ray(const olc::vf2d& t_screenPosition, const float t_radians, const RayType t_type)
    : screenPosition{ t_screenPosition }
    , radians{ t_radians }
    , type{ t_type }
{
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void Ray::Render(olc::PixelGameEngine* t_pge, const float t_xOffset, const float t_yOffset, const float t_scale) const
{
    const auto sx{ t_xOffset + screenPosition.x * t_scale };
    const auto sy{ t_yOffset + screenPosition.y * t_scale };
    const auto endPositionX{ sx + length * t_scale * cosf(radians) };
    const auto endPositionY{ sy + length * t_scale * sinf(radians) };

    t_pge->DrawLine(
        static_cast<int>(sx),
        static_cast<int>(sy),
        static_cast<int>(endPositionX),
        static_cast<int>(endPositionY),
        olc::RED
    );

    const auto shx{ t_xOffset + screenHitPosition.x * t_scale };
    const auto shy{ t_yOffset + screenHitPosition.y * t_scale };

    t_pge->DrawCircle(
        static_cast<int>(shx), static_cast<int>(shy),
        1,
        olc::GREEN
    );
}
