#pragma once
#include "Shape.h"

#include <vector>

class PolygonShape : public Shape
{
public:
  PolygonShape(std::vector<Point2f> points, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);
  PolygonShape(const PolygonShape& other);

  PolygonShape& operator=(const PolygonShape& other) = delete;
  PolygonShape(const PolygonShape&& other) = delete;
  PolygonShape& operator=(const PolygonShape&& other) = delete;
  virtual ~PolygonShape() override = default;

  virtual bool operator==(const Shape& other) const override;

  virtual void Draw(const Point2f& position) const override;

  virtual Shape* Copy() const override;

  Point2f GetSize() const override;

  std::vector<Point2f> GetPoints() const;
private:
  std::vector<Point2f> m_Points;
};