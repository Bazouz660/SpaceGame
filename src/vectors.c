/*
** EPITECH PROJECT, 2022
** fiziks
** File description:
** vectors.c
*/

#include "my.h"

sfVector2f vect_add(sfVector2f v1, sfVector2f v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

sfVector2f vect_sub(sfVector2f v1, sfVector2f v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

float vect_mag(sfVector2f v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sfVector2f vect_fmult(sfVector2f v, float n)
{
    v.x *= n;
    v.y *= n;
    return v;
}

sfVector2f vect_vmult(sfVector2f v, sfVector2f v2)
{
    v.x *= v2.x;
    v.y *= v2.y;
    return v;
}

sfVector2f vect_unit(sfVector2f v)
{
    if (vect_mag(v) == 0) {
        v.x = 0;
        v.y = 0;
        return v;
    }
    v.x = v.x / vect_mag(v);
    v.y = v.y / vect_mag(v);
    return v;
}

sfVector2f vect_norm(sfVector2f v)
{
    sfVector2f norm;

    norm.x = -v.y;
    norm.y = v.x;
    v = vect_unit(norm);
    return v;
}

float vect_dot(sfVector2f v1, sfVector2f v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

float vect_cross(sfVector2f v1, sfVector2f v2)
{
    return (v1.x * v2.y - v1.y * v2.x);
}

float dist_from(sfVector2f p1, sfVector2f p2)
{
    float distance = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));

    return (distance);
}

sfVector2f closest_point_seg(sfVector2f a, sfVector2f b, sfVector2f p)
{
    sfVector2f a_to_p = vect_sub(p, a);
    sfVector2f a_to_b = vect_sub(b, a);
    float atb2 = a_to_b.x * a_to_b.x + a_to_b.y * a_to_b.y;
    float atp_dot_atb = vect_dot(a_to_p, a_to_b);
    float t = atp_dot_atb / atb2;

    if (t > 1)
        t = 1;
    if (t < 0)
        t = 0;
    return (sfVector2f){a.x + a_to_b.x * t, a.y + a_to_b.y * t};
}

sfVector2f vect2u_vect2f(sfVector2u v)
{
    sfVector2f v2;

    v2.x = v.x;
    v2.y = v.y;
    return v2;
}

sfVector2f clamp_vect2f(sfVector2f v, float maxm)
{
    float length = vect_mag(v);
    float multiplier;

    if (length > maxm)
        multiplier = maxm / length;
    else
        multiplier = 1.0;
    return ((sfVector2f){v.x * multiplier, v.y * multiplier});
}