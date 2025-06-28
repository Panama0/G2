#include "gtest/gtest.h"

#include "Vec2.hpp"

TEST(VEC2_Tests, constructor)
{
    int first{5};
    int second{6};

    Vec2 test(first, second);

    ASSERT_EQ(test.x, first);
    ASSERT_EQ(test.y, second);
}

TEST(VEC2_Tests, boolean)
{
    Vec2 first{5.f, 5.f};
    Vec2 second{5.f, 5.f};

    ASSERT_EQ(first, second);
}

TEST(VEC2_Tests, adding)
{
    Vec2 first{5, 5};
    Vec2 second{5, 5};

    Vec2 result = first + second;
    Vec2 correctResult{10, 10};

    ASSERT_EQ(result, correctResult);
}

TEST(VEC2_Tests, multiplication)
{
    Vec2 first{5, 5};
    Vec2 second{5, 5};

    Vec2 result = first * second;
    Vec2 correctResult{25, 25};

    ASSERT_EQ(result, correctResult);
}

TEST(VEC2_Tests, scalar_mult)
{
    Vec2 first{5, 6};
    int scalar{10};

    Vec2 result = first * scalar;
    Vec2 correctResult{50, 60};

    ASSERT_EQ(result, correctResult);
}

TEST(VEC2_Tests, compoundAssignment)
{
    Vec2 first{5, 6};
    Vec2 second{5, 6};

    first += second;
    Vec2 correctResult{10, 12};

    ASSERT_EQ(first, correctResult);
}

TEST(VEC2_Tests, length)
{
    Vec2 vec{3.f, 4.f};
    Vec2 vec1{-3.f, -4.f};

    ASSERT_EQ(vec.length(), 5.f);
    ASSERT_EQ(vec1.length(), 5.f);
}

TEST(VEC2_Tests, normalised)
{
    Vec2 vec{3.f, 4.f};

    ASSERT_NEAR(vec.normalised().x, 0.6f, 0.0000f);
    ASSERT_NEAR(vec.normalised().y, 0.8f, 0.0000f);

    Vec2 zero{0.f, 0.f};
    ASSERT_EQ(zero.normalised(), zero);
}
