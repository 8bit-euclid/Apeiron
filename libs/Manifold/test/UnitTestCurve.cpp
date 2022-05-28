#include <gtest/gtest.h>

#include "../../../include/Global.h"
#include "../include/Curve.h"

#ifdef DEBUG_MODE

namespace aprn::mnfld {

/***************************************************************************************************************************************************************
* Curve Test Fixture
***************************************************************************************************************************************************************/
class CurveTest : public testing::Test
{
public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  CurveTest()
    : RandomInt(-10, 10), RandomFloat(-Ten, Ten) {}

  void
  SetUp() override
  {

  }
};


/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/
TEST_F(CurveTest, Line)
{
  SVectorF3 p, direction, norm_direction, centre;
  direction.Randomise();
  centre.Randomise();
  norm_direction = Normalise(direction);
  Line line(direction, centre);

  // Not unit speed parametrised
  p = line.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], centre[i]);

  p = line.Point(One);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], centre[i] + direction[i]);

  p = line.Point(-One);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], centre[i] - direction[i]);

  // Unit speed parametrised
  line.SetIfUnitSpeed(true);
  const Float random = RandomFloat();
  p = line.Point(random);
  FOR(i, 3) EXPECT_NEAR(p[i], centre[i] + random * norm_direction[i], Two * Small);

  p = line.Point(-random);
  FOR(i, 3) EXPECT_NEAR(p[i], centre[i] - random * norm_direction[i], Two * Small);
}

TEST_F(CurveTest, Ray)
{
  SVectorF3 p, start, direction, norm_direction;
  start.Randomise();
  direction.Randomise();
  norm_direction = Normalise(direction);
  Ray ray(direction, start);

  // Not unit speed parametrised
  p = ray.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], start[i]);

  p = ray.Point(One);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], start[i] + direction[i]);

  p = ray.Point(Two);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], start[i] + Two * direction[i]);

  EXPECT_THROW(ray.Point(-Small), std::domain_error);

  // Unit speed parametrised
  ray.SetIfUnitSpeed(true);
  RandomFloat.Reset(Zero, Ten);
  const Float random = RandomFloat();
  p = ray.Point(random);
  FOR(i, 3) EXPECT_NEAR(p[i], start[i] + random * norm_direction[i], Small);

  EXPECT_THROW(ray.Point(-Small), std::domain_error);
}

TEST_F(CurveTest, Segment)
{
  SVectorF3 p, start, end, direction, norm_direction;
  start.Randomise();
  end.Randomise();
  direction = end - start;
  norm_direction = Normalise(direction);
  const Float magnitude = Magnitude(direction);
  Segment segment(start, end);

  // Not unit speed parametrised
  p = segment.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], start[i]);

  p = segment.Point(One);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], end[i]);

  p = segment.Point(Half);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], start[i] + Half * direction[i]);

  EXPECT_THROW(segment.Point(-Ten * Small), std::domain_error);
  EXPECT_THROW(segment.Point(One + Ten * Small), std::domain_error);

  // Unit speed parametrised
  segment.SetIfUnitSpeed(true);
  RandomFloat.Reset(Zero, magnitude);
  const Float random = RandomFloat();
  p = segment.Point(random);
  FOR(i, 3) EXPECT_NEAR(p[i], start[i] + random * norm_direction[i], Small);

  EXPECT_THROW(segment.Point(-Ten * Small), std::domain_error);
  EXPECT_THROW(segment.Point(magnitude + Ten * Small), std::domain_error);
}

TEST_F(CurveTest, SegmentChain)
{
  RandomInt.Reset(3, 12);
  const size_t n_vertices = RandomInt();

  DynamicArray<SVectorF3> vertices(n_vertices);
  DynamicArray<SVectorF3> directions(n_vertices - 1);
  DynamicArray<SVectorF3> norm_directions(n_vertices - 1);
  DynamicArray<Float> magnitudes(n_vertices - 1);
  Float chain_length{};
  FOR(i, vertices.size())
  {
    vertices[i].Randomise();
    vertices[i] *= Ten;
    if(i > 0)
    {
      directions[i - 1] = vertices[i] - vertices[i - 1];
      norm_directions[i - 1] = Normalise(directions[i - 1]);
      magnitudes[i - 1] = Magnitude(directions[i - 1]);
      chain_length += magnitudes[i - 1];
    }
  }

  SegmentChain chain(vertices);
  SVectorF3 p;

  // Not unit speed parametrised
  p = chain.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], vertices.front()[i]);

  p = chain.Point(One);
  FOR(i, 3) EXPECT_NEAR(p[i], vertices.back()[i], Ten * Small);

  RandomFloat.Reset(Zero, One);
  Float random = RandomFloat();
  p = chain.Point(random);
  Float param_length = random * chain_length;
  Float sum{};
  SVectorF3 p_check;
  bool isfound{};
  FOR(i, magnitudes.size())
  {
    const auto sum_prev = sum;
    sum += magnitudes[i];
    if(param_length <= sum)
    {
      const Float r = (param_length - sum_prev) / magnitudes[i];
      p_check = (One - r) * vertices[i] + r * vertices[i + 1];
      isfound = true;
      break;
    }
  }
  EXPECT_TRUE(isfound);
  FOR(i, 3) EXPECT_NEAR(p[i], p_check[i], Two * Small);

  EXPECT_THROW(chain.Point(-Small), std::domain_error);
  EXPECT_THROW(chain.Point(One + Small), std::domain_error);

  // Unit speed parametrised
  chain.SetIfUnitSpeed(true);
  RandomFloat.Reset(Zero, chain_length);
  random = RandomFloat();
  p = chain.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], vertices.front()[i]);

  p = chain.Point(chain_length);
  FOR(i, 3) EXPECT_NEAR(p[i], vertices.back()[i], Ten * Small);

  RandomFloat.Reset(Zero, chain_length);
  random = RandomFloat();
  p = chain.Point(random);
  param_length = random;
  sum = Zero;
  isfound = false;
  FOR(i, magnitudes.size())
  {
    const auto sum_prev = sum;
    sum += magnitudes[i];
    if(param_length <= sum)
    {
      const Float r = (param_length - sum_prev) / magnitudes[i];
      p_check = (One - r) * vertices[i] + r * vertices[i + 1];
      isfound = true;
      break;
    }
  }
  EXPECT_TRUE(isfound);
  FOR(i, 3) EXPECT_NEAR(p[i], p_check[i], Two * Small);

  EXPECT_THROW(chain.Point(-Small), std::domain_error);
  EXPECT_THROW(chain.Point(chain_length + Ten * Small), std::domain_error);

  // Test closed chain
  chain = SegmentChain(vertices, true);
  p = chain.Point(Zero);
  FOR(i, 3) EXPECT_DOUBLE_EQ(p[i], vertices.front()[i]);

  p = chain.Point(One);
  FOR(i, 3) EXPECT_NEAR(p[i], vertices.front()[i], Ten * Small);
}

/***************************************************************************************************************************************************************
* Circular/Elliptical Curves
***************************************************************************************************************************************************************/
TEST_F(CurveTest, Circle)
{
  RandomFloat.Reset(One, Ten);
  const Float radius = RandomFloat();
  SVectorF2 p, p_check, centre;
  centre.Randomise();
  Circle circle(radius, centre);

  // Not unit speed parametrised
  p = circle.Point(Zero);
  FOR(i, 2) EXPECT_DOUBLE_EQ(p[i], centre[i] + radius * xAxis2[i]);

  p = circle.Point(QuartPi);
  FOR(i, 2) EXPECT_NEAR(p[i], centre[i] + radius / Sqrt(Two), Two * Small);

  p = circle.Point(HalfPi);
  FOR(i, 2) EXPECT_NEAR(p[i], centre[i] + radius * yAxis2[i], Two * Small);

  p = circle.Point(-HalfPi);
  FOR(i, 2) EXPECT_DOUBLE_EQ(p[i], centre[i] - radius * yAxis2[i]);

  // Unit speed parametrised
  circle.SetIfUnitSpeed(true);
  RandomFloat.Reset(-Ten, Ten);
  const Float random = RandomFloat();
  p = circle.Point(random);
  Float theta = random / radius;
  p_check = centre + radius * SVectorF2{Cos(theta), Sin(theta)};
  FOR(i, 2) EXPECT_NEAR(p[i], p_check[i], 30.0 * Small);

  p = circle.Point(-random);
  p_check = centre + radius * SVectorF2{Cos(theta), -Sin(theta)};
  FOR(i, 2) EXPECT_NEAR(p[i], p_check[i], 30.0 * Small);
}

TEST_F(CurveTest, Ellipse)
{
  RandomFloat.Reset(One, Ten);
  const Float radius_x = RandomFloat();
  const Float radius_y = RandomFloat();
  SVectorF2 p, p_check, centre;
  centre.Randomise();
  Ellipse ellipse(radius_x, radius_y, centre);

  // Not unit speed parametrised
  p = ellipse.Point(Zero);
  FOR(i, 2) EXPECT_DOUBLE_EQ(p[i], centre[i] + radius_x * xAxis2[i]);

  p = ellipse.Point(QuartPi);
  p_check = centre + SVectorF2{radius_x / Sqrt(Two), radius_y / Sqrt(Two)};
  FOR(i, 2) EXPECT_NEAR(p[i], p_check[i], Two * Small);

  p = ellipse.Point(HalfPi);
  FOR(i, 2) EXPECT_NEAR(p[i], centre[i] + radius_y * yAxis2[i], Two * Small);

  p = ellipse.Point(-HalfPi);
  FOR(i, 2) EXPECT_NEAR(p[i], centre[i] - radius_y * yAxis2[i], Two * Small);

  // Unit speed parametrised - requires root-finding and quadrature first.
}

}

#endif