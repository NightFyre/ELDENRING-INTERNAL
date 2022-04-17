#include <cmath>
struct Vector2
{
public:
	float x, y;
};

struct Vector4
{
public:
	float x, y, z, w;
};
//#pragma pack(push, 1)
//struct Vector2
//{
//public:
//	float x;
//	float y;
//
//	inline Vector2()
//		: x(0), y(0) {
//	}
//
//	inline Vector2(float x, float y) : x(x), y(y) {}
//
//	__forceinline Vector2 operator-(const Vector2& V) {
//		return Vector2(x - V.x, y - V.y);
//	}
//
//	__forceinline Vector2 operator+(const Vector2& V) {
//		return Vector2(x + V.x, y + V.y);
//	}
//
//	__forceinline Vector2 operator*(float Scale) const {
//		return Vector2(x * Scale, y * Scale);
//	}
//
//	__forceinline Vector2 operator/(float Scale) const {
//		const float RScale = 1.f / Scale;
//		return Vector2(x * RScale, y * RScale);
//	}
//
//	__forceinline Vector2 operator+(float A) const {
//		return Vector2(x + A, y + A);
//	}
//
//	__forceinline Vector2 operator-(float A) const {
//		return Vector2(x - A, y - A);
//	}
//
//	__forceinline Vector2 operator*(const Vector2& V) const {
//		return Vector2(x * V.x, y * V.y);
//	}
//
//	__forceinline Vector2 operator/(const Vector2& V) const {
//		return Vector2(x / V.x, y / V.y);
//	}
//
//	__forceinline float operator|(const Vector2& V) const {
//		return x * V.x + y * V.y;
//	}
//
//	__forceinline float operator^(const Vector2& V) const {
//		return x * V.y - y * V.x;
//	}
//
//	__forceinline Vector2& operator+=(const Vector2& v) {
//		x += v.x;
//		y += v.y;
//		return *this;
//	}
//
//	__forceinline Vector2& operator-=(const Vector2& v) {
//		x -= v.x;
//		y -= v.y;
//		return *this;
//	}
//
//	__forceinline Vector2& operator*=(const Vector2& v) {
//		x *= v.x;
//		y *= v.y;
//		return *this;
//	}
//
//	__forceinline Vector2& operator/=(const Vector2& v) {
//		x /= v.x;
//		y /= v.y;
//		return *this;
//	}
//
//	__forceinline bool operator==(const Vector2& src) const {
//		return (src.x == x) && (src.y == y);
//	}
//
//	__forceinline bool operator!=(const Vector2& src) const {
//		return (src.x != x) || (src.y != y);
//	}
//
//	//__forceinline Vector2 Rotate(float angle)
//	//{
//	//	return Vector2(x * cos(-angle) - y * sin(-angle), y, x * sin(-angle) - y * cos(-angle));
//	//}
//
//	//__forceinline float Size() const {
//	//	return sqrt(x * x + y * y);
//	//}
//
//	//__forceinline float DistTo(Vector2 targetTo) const {
//	//	return (targetTo - *this).Size();
//	//}
//
//	//__forceinline Vector2 toRotator(Vector2 targetTo)
//	//{
//	//	Vector2 Normalized = (targetTo - *this);
//	//	return Normalized * (1 / Normalized.Size());
//	//}
//};
//#pragma pack(pop)

#pragma pack(push, 1)
struct Vector3
{
public:
	float x;
	float y;
	float z;

	inline Vector3()
		: x(0), y(0), z(0) {
	}

	inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	__forceinline Vector3 operator-(const Vector3& V) {
		return Vector3(x - V.x, y - V.y, z - V.z);
	}

	__forceinline Vector3 operator+(const Vector3& V) {
		return Vector3(x + V.x, y + V.y, z + V.z);
	}

	__forceinline Vector3 operator*(float Scale) const {
		return Vector3(x * Scale, y * Scale, z * Scale);
	}

	__forceinline Vector3 operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return Vector3(x * RScale, y * RScale, z * RScale);
	}

	__forceinline Vector3 operator+(float A) const {
		return Vector3(x + A, y + A, z + A);
	}

	__forceinline Vector3 operator-(float A) const {
		return Vector3(x - A, y - A, z - A);
	}

	__forceinline Vector3 operator*(const Vector3& V) const {
		return Vector3(x * V.x, y * V.y, z * V.z);
	}

	__forceinline Vector3 operator/(const Vector3& V) const {
		return Vector3(x / V.x, y / V.y, z / V.z);
	}

	__forceinline float operator|(const Vector3& V) const {
		return x * V.x + y * V.y + z * V.z;
	}

	__forceinline float operator^(const Vector3& V) const {
		return x * V.y - y * V.x - z * V.z;
	}

	__forceinline Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	__forceinline Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	__forceinline Vector3& operator*=(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	__forceinline Vector3& operator/=(const Vector3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	__forceinline bool operator==(const Vector3& src) const {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	__forceinline bool operator!=(const Vector3& src) const {
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	__forceinline Vector3 Rotate(float angle)
	{
		return Vector3(x * cos(-angle) - z * sin(-angle), y, x * sin(-angle) + z * cos(-angle));
	}

	__forceinline float Size() const {
		return sqrt(x * x + y * y + z * z);
	}

	__forceinline float DistTo(Vector3 targetTo) const {
		return (targetTo - *this).Size();
	}

	__forceinline Vector3 toRotator(Vector3 targetTo)
	{
		Vector3 Normalized = (targetTo - *this);
		return Normalized * (1 / Normalized.Size());
	}
};
#pragma pack(pop)

//#pragma pack(push, 1)
//struct Vector4
//{
//public:
//	float x;
//	float y;
//	float z;
//	float w;
//	inline Vector4()
//		: x(0), y(0), z(0), w(0) {
//	}
//
//	inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
//
//	__forceinline Vector4 operator-(const Vector4& V) {
//		return Vector4(x - V.x, y - V.y, z - V.z, w - V.w);
//	}
//
//	__forceinline Vector4 operator+(const Vector4& V) {
//		return Vector4(x + V.x, y + V.y, z + V.z, w + V.w);
//	}
//
//	__forceinline Vector4 operator*(float Scale) const {
//		return Vector4(x * Scale, y * Scale, z * Scale, w * Scale);
//	}
//
//	__forceinline Vector4 operator/(float Scale) const {
//		const float RScale = 1.f / Scale;
//		return Vector4(x * RScale, y * RScale, z * RScale, w * RScale);
//	}
//
//	__forceinline Vector4 operator+(float A) const {
//		return Vector4(x + A, y + A, z + A, w + A);
//	}
//
//	__forceinline Vector4 operator-(float A) const {
//		return Vector4(x - A, y - A, z - A, w - A);
//	}
//
//	__forceinline Vector4 operator*(const Vector4& V) const {
//		return Vector4(x * V.x, y * V.y, z * V.z, w * V.z);
//	}
//
//	__forceinline Vector4 operator/(const Vector4& V) const {
//		return Vector4(x / V.x, y / V.y, z / V.z, w / V.z);
//	}
//
//	__forceinline float operator|(const Vector4& V) const {
//		return x * V.x + y * V.y + z * V.z + w * V.w;
//	}
//
//	__forceinline float operator^(const Vector4& V) const {
//		return x * V.y - y * V.x - z * V.z - w * V.w;
//	}
//
//	__forceinline Vector4& operator+=(const Vector4& v) {
//		x += v.x;
//		y += v.y;
//		z += v.z;
//		w += v.w;
//		return *this;
//	}
//
//	__forceinline Vector4& operator-=(const Vector4& v) {
//		x -= v.x;
//		y -= v.y;
//		z -= v.z;
//		w -= v.w;
//		return *this;
//	}
//
//	__forceinline Vector4& operator*=(const Vector4& v) {
//		x *= v.x;
//		y *= v.y;
//		z *= v.z;
//		w *= v.w;
//		return *this;
//	}
//
//	__forceinline Vector4& operator/=(const Vector4& v) {
//		x /= v.x;
//		y /= v.y;
//		z /= v.z;
//		w /= v.w;
//		return *this;
//	}
//
//	__forceinline bool operator==(const Vector4& src) const {
//		return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
//	}
//
//	__forceinline bool operator!=(const Vector4& src) const {
//		return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
//	}
//
//	//__forceinline Vector4 Rotate(float angle)
//	//{
//	//	return Vector4(x * cos(-angle) - z * sin(-angle), y, x * sin(-angle) + z * cos(-angle));
//	//}
//
//	//__forceinline float Size() const {
//	//	return sqrt(x * x + y * y + z * z);
//	//}
//
//	//__forceinline float DistTo(Vector4 targetTo) const {
//	//	return (targetTo - *this).Size();
//	//}
//
//	//__forceinline Vector4 toRotator(Vector4 targetTo)
//	//{
//	//	Vector4 Normalized = (targetTo - *this);
//	//	return Normalized * (1 / Normalized.Size());
//	//}
//};
//#pragma pack(pop)