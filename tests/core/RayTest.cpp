#include <gtest/gtest.h>
#include "Ray.h"

#include <glm/glm.hpp>

class RayTest : public testing::Test
{
protected:
    void SetUp() override
    {
        m_ray.m_origin = glm::vec3{0.0f, 0.0f, 0.0f};
        m_ray.m_direction = glm::vec3{3.0f, 2.1f, 4.0f};
    }

    // void TearDown() override
    // {

    // }

    testing::AssertionResult IsParametricEquationCorrect()
    {
        float len = glm::length(m_ray(2));

        if((len >= 10.84f) && (len < 10.85f))
        {
            std::cout << m_ray << std::endl;

            return testing::AssertionSuccess();
        }
        else
        {
            return testing::AssertionFailure() << "len = " << len << " ====> " << m_ray;
        }
    }

    Ray m_ray;
};

TEST_F(RayTest, TestParametricEq)
{
    EXPECT_TRUE(IsParametricEquationCorrect());
}

// TEST_F(RayTest, TestParametricEq)
// {
//     glm::vec3 P = m_ray(2);
//     float len = glm::length(P);
//     EXPECT_EQ(10.84f, m_ray.m_origin);
// }