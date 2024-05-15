#pragma once
#include "Shape.h"

class CircleShape : public Shape
{
public:
  CircleShape(float radius, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);
  CircleShape(const CircleShape& other);

  CircleShape& operator=(const CircleShape& other) = delete;
  CircleShape(const CircleShape&& other) = delete;
  CircleShape& operator=(const CircleShape&& other) = delete;
  virtual ~CircleShape() override = default;

  virtual bool operator==(const Shape& other) const override;

  virtual void Draw(const Point2f& position) const override;

  virtual Shape* Copy() const override;

  Point2f GetSize() const override;

  float GetRadius() const;
private:
  float m_Radius;
};