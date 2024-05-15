#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
public:
  RectangleShape(const Point2f& size, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);
  RectangleShape(const RectangleShape& other);

  RectangleShape& operator=(const RectangleShape& other) = delete;
  RectangleShape(const RectangleShape&& other) = delete;
  RectangleShape& operator=(const RectangleShape&& other) = delete;
  virtual ~RectangleShape() override = default;

  virtual bool operator==(const Shape& other) const override;

  virtual void Draw(const Point2f& position) const override;

  virtual Point2f GetSize() const override;
  virtual Shape* Copy() const override;
private:
  Point2f m_Size;
};