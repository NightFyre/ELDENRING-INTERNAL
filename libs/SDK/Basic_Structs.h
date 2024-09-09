#pragma once
#pragma pack(push, 0x01)
namespace HEXINTON
{
    struct Vector2
    {
    public:
        float x, y;
    };

    struct Vector3
    {
    public:
        float x;
        float y;
        float z;

        inline Vector3() : x(0), y(0), z(0) { }
        inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator-(const Vector3& V) { return Vector3(x - V.x, y - V.y, z - V.z); }

        Vector3 operator+(const Vector3& V) { return Vector3(x + V.x, y + V.y, z + V.z); }

        Vector3 operator*(float Scale) const { return Vector3(x * Scale, y * Scale, z * Scale); }

        Vector3 operator+(float A) const { return Vector3(x + A, y + A, z + A); }

        Vector3 operator-(float A) const { return Vector3(x - A, y - A, z - A); }

        Vector3 operator*(const Vector3& V) const { return Vector3(x * V.x, y * V.y, z * V.z); }

        Vector3 operator/(const Vector3& V) const { return Vector3(x / V.x, y / V.y, z / V.z); }

        float operator|(const Vector3& V) const { return x * V.x + y * V.y + z * V.z; }

        float operator^(const Vector3& V) const { return x * V.y - y * V.x - z * V.z; }

        bool operator==(const Vector3& src) const { return (src.x == x) && (src.y == y) && (src.z == z); }

        bool operator!=(const Vector3& src) const { return (src.x != x) || (src.y != y) || (src.z != z); }


        Vector3 operator/(float Scale) const 
        {
            const float RScale = 1.f / Scale;
            return Vector3(x * RScale, y * RScale, z * RScale);
        }

        Vector3& operator+=(const Vector3& v) 
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& v) 
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3& operator*=(const Vector3& v) 
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        Vector3& operator/=(const Vector3& v) 
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        Vector3 Rotate(float angle) { return Vector3(x * cos(-angle) - z * sin(-angle), y, x * sin(-angle) + z * cos(-angle)); }

        float Size() const { return sqrt(x * x + y * y + z * z); }

        float DistTo(Vector3 targetTo) const { return (targetTo - *this).Size(); }

        Vector3 toRotator(Vector3 targetTo)
        {
            Vector3 Normalized = (targetTo - *this);
            return Normalized * (1 / Normalized.Size());
        }
    };

    struct Vector4
    {
    public:
        float x, y, z, w;
    };
}
#pragma pack(pop)