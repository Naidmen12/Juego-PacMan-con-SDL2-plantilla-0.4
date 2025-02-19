#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
public:
    float x;
    float y;

    Vector2D() : x(0), y(0) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D& operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& v) const {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D& operator-=(const Vector2D& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float Magnitud() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2D Normalizado() const {
        float mag = Magnitud();
        if (mag > 0) {
            return Vector2D(x / mag, y / mag);
        }
        return Vector2D(0, 0);
    }

    static float Distancia(const Vector2D& v1, const Vector2D& v2) {
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool EsCero() const {
        return x == 0 && y == 0;
    }
};

#endif // VECTOR2D_H

