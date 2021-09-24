#pragma once

#if defined (__SSE4_2__) || defined (__SSE4_1__)
#include <x86intrin.h>
#elif (defined(__arm) || defined(__arm__))
#include <sse2neon.h>
#else
#error SSE4 instrinsics implementation not found
#endif

/**
 * 16-bytes aligned memory allocation function.
 * @param size Size of the memory chunk to allocate in bytes.
 * @return A pointer to the newly aligned memory, or nullptr.
 */
void *mallocSimd(size_t size);

/**
 * 16-bytes aligned memory free function.
 * @param ptr Memory pointer to free, which must have been allocated using mallocSimd.
 */
void freeSimd(void *ptr);

/**
** 3D floating-point precision mathematical vector class.
*/
#ifdef __GNUC__
class __attribute__((aligned (16))) Vector3
#else
  _MM_ALIGN16 class Vector3
#endif
{
public:
  inline Vector3() : value(_mm_setzero_ps()) {};
  inline Vector3(float x, float y, float z) : value(_mm_set_ps(0, z, y, x)) {}
  inline explicit Vector3(__m128 m) : value(m) {}

  /*
   * Arithmetic operators with Vector3
   */
  inline Vector3 operator+(const Vector3 &b) const { return Vector3(_mm_add_ps(value, b.value)); }
  inline Vector3 operator-(const Vector3 &b) const { return Vector3(_mm_sub_ps(value, b.value)); }
  inline Vector3 operator*(const Vector3 &b) const { return Vector3(_mm_mul_ps(value, b.value)); }
  inline Vector3 operator/(const Vector3 &b) const { return Vector3(_mm_div_ps(value, b.value)); }

  /*
   * Assignation and arithmetic operators with Vector3
   */
  inline Vector3 &operator+=(const Vector3 &b) {
    value = _mm_add_ps(value, b.value);
    return *this;
  }
  inline Vector3 &operator-=(const Vector3 &b) {
    value = _mm_sub_ps(value, b.value);
    return *this;
  }
  inline Vector3 &operator*=(const Vector3 &b) {
    value = _mm_mul_ps(value, b.value);
    return *this;
  }
  inline Vector3 &operator/=(const Vector3 &b) {
    value = _mm_div_ps(value, b.value);
    return *this;
  }

  /*
   * Arithmetic operators with floats
   */
  inline Vector3 operator+(float b) const { return Vector3(_mm_add_ps(value, _mm_set1_ps(b))); }
  inline Vector3 operator-(float b) const { return Vector3(_mm_sub_ps(value, _mm_set1_ps(b))); }
  inline Vector3 operator*(float b) const { return Vector3(_mm_mul_ps(value, _mm_set1_ps(b))); }
  inline Vector3 operator/(float b) const { return Vector3(_mm_div_ps(value, _mm_set1_ps(b))); }

  /*
   * Assignation and arithmetic operators with float
   */
  inline Vector3 &operator+=(float b) {
    value = _mm_add_ps(value, _mm_set1_ps(b));
    return *this;
  }
  inline Vector3 &operator-=(float b) {
    value = _mm_sub_ps(value, _mm_set1_ps(b));
    return *this;
  }
  inline Vector3 &operator*=(float b) {
    value = _mm_mul_ps(value, _mm_set1_ps(b));
    return *this;
  }
  inline Vector3 &operator/=(float b) {
    value = _mm_div_ps(value, _mm_set1_ps(b));
    return *this;
  }

  /*
   * Equality operators
   */
  inline bool operator==(const Vector3 &b) const {
    return (((_mm_movemask_ps(_mm_cmpeq_ps(value, b.value))) & 0x7) == 0x7);
  }
  inline bool operator!=(const Vector3 &b) const { return !(*this == b); }

  /*
   * Unary minus operator
   */
  inline Vector3 operator-() const { return Vector3(_mm_xor_ps(value, _signMask)); }

  /**
   * Cross product
   * @param b Other vector
   * @return Cross product
   */
  [[nodiscard]] inline Vector3 cross(const Vector3 &b) const {
    __m128 result = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(value, value, _MM_SHUFFLE(3, 0, 2, 1)),
                   _mm_shuffle_ps(b.value, b.value, _MM_SHUFFLE(3, 1, 0, 2))),
        _mm_mul_ps(_mm_shuffle_ps(value, value, _MM_SHUFFLE(3, 1, 0, 2)),
                   _mm_shuffle_ps(b.value, b.value, _MM_SHUFFLE(3, 0, 2, 1)))
    );
    return Vector3(result);
  }

  /**
   * Dot product
   * @param b Other vector
   * @return Dot product
   */
  [[nodiscard]] inline float dot(const Vector3 &b) const { return _mm_cvtss_f32(_mm_dp_ps(value, b.value, 0x71)); }

  /**
   * Length of the vector
   */
  [[nodiscard]] inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(value, value, 0x71))); }

  /**
   * Get normalized vector
   */
  [[nodiscard]] inline Vector3 normalize() const {
    // multiplying by rsqrt does not yield an accurate enough result, so divide by sqrt instead.
    return Vector3(_mm_div_ps(value, _mm_sqrt_ps(_mm_dp_ps(value, value, 0xFF))));
  }

  /*
   * Overloaded new operator that ensure alignment
   */
  inline void *operator new[](size_t sz) { return mallocSimd(sz); }

  /*
   * Overloaded delete operator that ensure alignment
   */
  inline void operator delete[](void *ptr) {
    if (ptr) {
      freeSimd(ptr);
    }
  }

  /*
   * Textual representation
   */
  friend std::ostream &operator<<(std::ostream &os, const Vector3 &v);

  /*
   * Direct access member variables.
   */
  union {
    struct {
      __m128 value;
    };
    struct {
      float x, y, z;
    };
  };

private:
  /**
   * __m128 bits mask to target the floating point sign bit.
   */
  const __m128 _signMask = _mm_castsi128_ps(_mm_set1_epi32(static_cast<int32_t>(0x80000000)));
};

/**
 * External operators that maps to the actual Vector3 method.
 */
inline Vector3 operator+(float a, const Vector3 &b) { return b + a; }
inline Vector3 operator-(float a, const Vector3 &b) { return Vector3(_mm_set1_ps(a)) - b; }
inline Vector3 operator*(float a, const Vector3 &b) { return b * a; }
inline Vector3 operator/(float a, const Vector3 &b) { return Vector3(_mm_set1_ps(a)) / b; }

