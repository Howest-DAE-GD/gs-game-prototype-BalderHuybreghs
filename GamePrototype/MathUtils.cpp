#include "pch.h"
#include "MathUtils.h"

int MathUtils::RandInt(int min, int max)
{
  int scaledMax{ (max - min) + 1 };
  return rand() % scaledMax + min;
}

float MathUtils::RandFloat(float min, float max, int decimals)
{
  int scale{ (int)pow(10, decimals) };
  int scaledMax{ int((max - min) * scale) };
  return rand() % (scaledMax + 1) / float(scale) + min;
}

float MathUtils::Lerp(float start, float end, float t)
{
  t = std::max(0.0f, std::min(1.0f, t));
  return start + t * (end - start);
}

float MathUtils::Slerp(float start, float end, float t)
{
  t = std::max(0.0f, std::min(1.0f, t));

  // Compute the cosine of the angle between the two values
  float cosTheta = start * end + cosf((end - start) * t);

  // If the angle is small, use linear interpolation
  if (cosTheta > 0.9995f)
  {
    return Lerp(start, end, t);
  } else
  {
    // Use spherical interpolation
    float theta = acosf(cosTheta);
    float sinTheta = sinf(theta);
    float weightA = sinf((1 - t) * theta) / sinTheta;
    float weightB = sinf(t * theta) / sinTheta;
    return start * weightA + end * weightB;
  }
}

float MathUtils::Wave(float amplitude, float period, float phase, float shift, float t)
{
  return amplitude * sinf((2 * float(M_PI)) / period * (t + phase)) + shift;
}

bool MathUtils::RandBool()
{
    return RandInt(0, 1) == 1;
}

Point2f MathUtils::Lerp(const Point2f& a, const Point2f& b, float t)
{
  return Point2f{
    Lerp(a.x, b.x, t),
    Lerp(a.y, b.y, t),
  };
}

Point2f MathUtils::Slerp(const Point2f& a, const Point2f& b, float t)
{
  return Point2f{
    Slerp(a.x, b.x, t),
    Slerp(a.y, b.y, t),
  };
}

Point2f MathUtils::RandPoint(const Rectf& range, int decimals)
{
  float randX = RandFloat(range.left, range.left + range.width, decimals);
  float randY = RandFloat(range.bottom, range.bottom + range.height, decimals);
  return Point2f(randX, randY);
}

Point2f MathUtils::RandPoint(const Circlef& range, int decimals)
{
  float randAngle = RandFloat(0.0f, 2 * float(M_PI), decimals); // A random angle
  float randRadius = RandFloat(0.0f, range.radius, decimals); // Random radius

  // Convert polar to cartesian coordinates
  float randX = range.center.x + randRadius * cosf(randAngle);
  float randY = range.center.y + randRadius * sinf(randAngle);

  return Point2f(randX, randY);
}

Point2f MathUtils::RandPoint(float min, float max, int decimals)
{
  return Point2f(
    RandFloat(min, max, decimals),
    RandFloat(min, max, decimals)
  );
}

Color4f MathUtils::RandColor(const Color4f& min, const Color4f& max, int decimals)
{
  return Color4f(
    RandFloat(min.r, max.r, decimals),
    RandFloat(min.g, max.g, decimals),
    RandFloat(min.b, max.b, decimals),
    RandFloat(min.a, max.a, decimals)
  );
}

Color4f MathUtils::RandColor(int decimals)
{
  return RandColor(Color4f{ 0.f, 0.f, 0.f, 1.f }, Color4f{ 1.f, 1.f, 1.f, 1.f }, decimals);
}
