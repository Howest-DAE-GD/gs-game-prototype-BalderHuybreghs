#pragma once
#include "structs.h"

class Shape
{
public:
  virtual ~Shape() = default;

  virtual void Draw(const Point2f& position) const = 0;

  // Creates a copy of the current shape, don't forget to delete the copy :)
  virtual Shape* Copy() const = 0;
  virtual bool operator==(const Shape& other) const = 0;

  virtual Point2f GetSize() const = 0;
  Color4f GetColor() const;
  bool    IsFilled() const;

  void SetColor(const Color4f& color);
  void SetFilled(bool filled);
protected:
  Shape(const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);

  Color4f m_Color;
  bool    m_Filled;
};