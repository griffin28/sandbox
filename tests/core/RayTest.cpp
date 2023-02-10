#include <gtest/gtest.h>
#include "Ray.h"

#include <glm/gtx/norm.hpp>

class RayTest : public testing::Test
{
protected:
    void SetUp() override
    {
        m_ray.m_origin = glm::vec3{0.0f, 0.0f, 0.0f};
        m_ray.m_direction = glm::vec3{3.0f, 2.1f, 4.0f};
    }

    testing::AssertionResult IsParametricEquationCorrect()
    {
        float len = glm::length2(m_ray(2));

        if(len == 117.64f)
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