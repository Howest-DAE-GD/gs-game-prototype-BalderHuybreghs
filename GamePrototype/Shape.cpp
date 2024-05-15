#include "pch.h"
#include "Shape.h"
#include "utils.h"

Shape::Shape(const Color4f& color, bool filled)
  : m_Color(color), m_Filled(filled)
{
}

void Shape::SetColor(const Color4f& color)
{
  m_Color = color;
}

void Shape::SetFilled(bool filled)
{
  m_Filled = filled;
}

Color4f Shape::GetColor() const
{
  return m_Color;
}

bool Shape::IsFilled() const
{
  return m_Filled;
}
