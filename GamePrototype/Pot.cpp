#include "pch.h"
#include "Pot.h"
#include "MathUtils.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "utils.h"
#include "PolygonShape.h"
#include "HealthAbility.h"
#include "IngredientAbility.h"
#include "SlowAbility.h"

#include <cmath>

Pot::Pot(const Rectf& rectangle, int ingredients, int variations, float viewingTime, float craftingTime, float showingTime, float selectionTime)
  : m_State(State::View), m_Rectangle(rectangle), m_Ingredients(ingredients), m_Variations(variations), m_ViewingTime(viewingTime), m_ShowingTime(showingTime), m_SelectionTime(selectionTime), m_CraftingTime(craftingTime), m_SineTime(0), m_Result(0)
{
  m_Texture = new Texture("textures/pot.png");

  m_Craftables.reserve(m_Variations);
  m_Shapes.reserve(ingredients);

  m_Abilities.push_back(new HealthAbility(1));
  m_Abilities.push_back(new IngredientAbility(1));
  m_Abilities.push_back(new SlowAbility(1.f));

  GenerateRecipe();
}

Pot::~Pot()
{
  delete m_Texture;

  for (Ability* ability : m_Abilities) {
    delete ability;
    ability = nullptr;
  }

  Flush();
}

void Pot::Draw(float screenWidth) const
{
  switch (m_State) {
    case State::View: {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      for (size_t i = 0; i < m_Shapes.size(); i++)
      {
        m_Shapes[i]->Draw(
          Point2f{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 50.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Shapes.size()), 0.f, m_SineTime)
          });
      }

      break;
    }

    case State::Craft:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      float timePerShape{ (m_CraftingTime / m_Shapes.size()) };
      float percentage{ m_Time / m_CraftingTime };

      int shapesToHide{ int(percentage * m_Shapes.size()) };
      float lerpProgress{ (m_Time - shapesToHide * timePerShape) / timePerShape };

      for (int i = m_Shapes.size() -1; i >= shapesToHide; i--)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 50.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Shapes.size()), 0.f, m_SineTime)
        };

        if (i == shapesToHide) {
          m_Shapes[i]->Draw(MathUtils::Lerp(pos, m_Rectangle.Center(), lerpProgress));
          continue;
        }

        m_Shapes[i]->Draw(pos);
      }

      break;
    }

    case State::Show:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      float timePerShape{ m_ShowingTime / m_Craftables.size() };
      int currentShapeIndex = int(m_Time / timePerShape);
      float lerpProgress{ (m_Time - currentShapeIndex * timePerShape) / timePerShape };

      for (int i = 0; i <= currentShapeIndex && i < m_Craftables.size(); i++)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 50.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Craftables.size()), 0.f, m_SineTime)
        };

        if (i == currentShapeIndex) {
          DrawResult(i, MathUtils::Lerp(m_Rectangle.Center(), pos, lerpProgress));
          continue;
        }

        DrawResult(i, pos);
      }

      break;
    }

    case State::Select:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      for (int i = 0; i < m_Craftables.size(); i++)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 50.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Craftables.size()), 0.f, m_SineTime)
        };

        DrawResult(i, pos);
      }

      break;
    }

    case State::ShowAbility:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      float timePerAbility{ m_ShowingTime / m_Abilities.size() };
      int currentAbilityIndex = int(m_Time / timePerAbility);
      float lerpProgress{ (m_Time - currentAbilityIndex * timePerAbility) / timePerAbility };

      for (int i = 0; i <= currentAbilityIndex && i < m_Abilities.size(); i++)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 50.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Abilities.size()), 0.f, m_SineTime)
        };

        Point2f p{ MathUtils::Lerp(m_Rectangle.Center(), pos, lerpProgress) };
        if (i == currentAbilityIndex) {
          m_Abilities[i]->Draw(Rectf{ p.x, p.y, 60.f, 60.f });
          continue;
        }

        m_Abilities[i]->Draw(Rectf{ p.x, p.y, 60.f, 60.f });
      }

      break;
    }

    case State::Ability:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Abilities.size() - 1)) / 2.f)
      };

      // Check which shape was selected
      for (int i = 0; i < m_Abilities.size(); i++)
      {
        const Rectf area{
          (x + 60.f * i) - 30.f,
          m_Rectangle.bottom + m_Rectangle.height + 50.f - 30.f + MathUtils::Wave(10.f, 5.f, i * (M_PI / m_Abilities.size()), 0.f, m_SineTime),
          60.f,
          60.f
        };

        m_Abilities[i]->Draw(area);
      }
    }
  }

  m_Texture->Draw(m_Rectangle);
}

bool Pot::Update(float elapsedSec)
{
  bool val{};

  switch (m_State) {
    case State::View:
    {
      if (m_Time > m_ViewingTime) {
        m_Time = 0;
        m_State = State::Craft;
      }

      break;
    }

    case State::Craft:
    {
      if (m_Time > m_CraftingTime) {
        m_Time = 0;
        m_State = State::Show;
      }

      break;
    }

    case State::Show:
    {
      if (m_Time > m_ShowingTime) {
        m_Time = 0;
        m_State = State::Select;
      }

      break;
    }
    case State::Select:
    {

      if (m_Time > m_SelectionTime) {
        m_Time = 0;
        m_State = State::View;
        GenerateRecipe();
        val = true;
      }

      break;
    }
    case State::ShowAbility:
    {
      if (m_Time > m_ShowingTime) {
        m_Time = 0;
        m_State = State::Ability;
      }

      break;
    }
  }

  m_Time += elapsedSec;
  m_SineTime += elapsedSec;
  return val;
}

void Pot::GenerateRecipe()
{
  Flush(); // Flush the previous recipe

  std::vector<Shape*> shapes{ GetRandomShapes(m_Ingredients) };
  std::vector<std::vector<Shape*>> craftables{ };
  craftables.reserve(m_Variations);

  m_Result = MathUtils::RandInt(0, m_Variations -1);

  for (int i = 0; i < m_Variations; i++)
  {
    if (i != m_Result) {
      craftables.push_back(GetRandomShapes(m_Ingredients));
      continue;
    }

    std::vector<Shape*> copiedShapes{};
    copiedShapes.reserve(shapes.size());
    for (const Shape* shapePtr : shapes) {
      copiedShapes.push_back(shapePtr->Copy());
    }

    craftables.push_back(copiedShapes);
  }

  m_Shapes = shapes;
  m_Craftables = craftables;
}

void Pot::Flush()
{
  m_State = State::View;
  m_Time = 0.f;
  m_SineTime = 0.f;
  m_Result = 0;

  for (Shape* shape : m_Shapes) {
    delete shape;
    shape = nullptr;
  }

  m_Shapes.clear();

  for (std::vector<Shape*> craftable : m_Craftables) {
    for (Shape* shape : craftable) {
      delete shape;
      shape = nullptr;
    }
  }

  m_Craftables.clear();
}

std::vector<Shape*> Pot::GetRandomShapes(int size) const
{
  std::vector<Shape*> shapes{};
  shapes.reserve(size);

  for (size_t i = 0; i < size; i++)
  {
    shapes.push_back(GetRandomShape());
  }

  return shapes;
}

Shape* Pot::GetRandomShape() const
{
  // Min and max colors to make sure the colors are always clearly visible and distinguishable
  Color4f colorMin{ Color4f{ 0, 0, 0, 1.f } };
  Color4f colorMax{ Color4f{ .9f, .9f, .9f, 1.f } };

  bool filled{ MathUtils::RandBool() };
  Color4f color{ MathUtils::RandColor(colorMin, colorMax, 3) };
  int shape{ MathUtils::RandInt(0, 2) };

  switch (shape) {
    case 0:
    {
      const Point2f size{ MathUtils::RandPoint(10.f, 50.f, 2) };

      return new RectangleShape(
        size,
        color,
        filled
      );
    }

    case 1:
    {
      // Triangle
      float baseLength{ MathUtils::RandFloat(25.f, 50.f, 2.f) };
      // float rotation{ MathUtils::RandFloat(0.f, M_PI * 2, 5.f) };
      float rotation{};

      // Calculate the height of the equilateral triangle based on the base length
      float height = baseLength * sqrt(3.f) / 2.f;

      std::vector<Point2f> points{
        // Bottom left point
        Point2f{-baseLength * 0.5f, -height * 0.5f},
        // Bottom right point
        Point2f{baseLength * 0.5f, -height * 0.5f},
        // Top point
        Point2f{0.f, height * 0.5f}
      };

      // Rotate the points by the specified angle
      for (Point2f& point : points) {
        float x = point.x * cos(rotation) - point.y * sin(rotation);
        float y = point.x * sin(rotation) + point.y * cos(rotation);
        point = Point2f{ x, y };
      }

      return new PolygonShape(
        points,
        color,
        filled
      );
    }

    default:
    {
      const float radius{ MathUtils::RandFloat(5.f, 25.f, 2) };

      return new CircleShape(
        radius,
        color,
        filled
      );
    }
  }
}

void Pot::DrawResult(int id, const Point2f& position) const
{
  const std::vector<Shape*> result{ m_Craftables[id] };
  for (const Shape* shape : result) {
    shape->Draw(position);
  }
}

Rectf Pot::GetRect() const
{
  return m_Rectangle;
}

Pot::State Pot::GetState() const
{
  return m_State;
}

int Pot::GetIngredients() const
{
  return m_Ingredients;
}

int Pot::GetVariations() const
{
  return m_Variations;
}

float Pot::GetViewingTime() const
{
  return m_ViewingTime;
}

void Pot::SetIngredients(int ingredients)
{
  m_Ingredients = ingredients;
}

void Pot::SetVariations(int variations)
{
  m_Variations = variations;
}

void Pot::SetViewingTime(float viewingTime)
{
  m_ViewingTime = viewingTime;
}

void Pot::ShowAbilities()
{
  m_State = State::ShowAbility;
}

float Pot::GetTime() const
{
    return m_Time;
}

int Pot::GetShapeId(float screenWidth, const Point2f& position) const
{
  float x{
    (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
  };

  // Check which shape was selected
  for (int i = 0; i < m_Craftables.size(); i++)
  {
    const Rectf area{
      (x + 60.f * i) - 30.f,
      m_Rectangle.bottom + m_Rectangle.height + 50.f - 30.f,
      60.f,
      60.f
    };

    if (utils::IsPointInRect(position, area)) {
      return i;
    }
  }

  return -1;
}

Ability* Pot::GetAbility(float screenWidth, const Point2f& position) const
{
  float x{
    (screenWidth / 2.f) - ((60.f * (m_Abilities.size() - 1)) / 2.f)
  };

  // Check which shape was selected
  for (int i = 0; i < m_Abilities.size(); i++)
  {
    const Rectf area{
      (x + 60.f * i) - 30.f,
      m_Rectangle.bottom + m_Rectangle.height + 50.f - 30.f,
      60.f,
      60.f
    };

    if (utils::IsPointInRect(position, area)) {
      return m_Abilities[i];
    }
  }

  return nullptr;
}

bool Pot::IsSelected(int id) const
{
  return id == m_Result;
}
