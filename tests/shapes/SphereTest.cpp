#include <gtest/gtest.h>
#include "sphere.h"

class SphereTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        m_sphere.setRadius(5.1f);
        m_sphere.setCenter(2.1f,2.2f,2.3f);
    }

    // void TearDown() override
    // {

    // }

    Sphere m_sphere;
};

TEST_F(SphereTest, TestRadius)
{
    EXPECT_EQ(5.1f, m_sphere.getRadius());
}

TEST_F(SphereTest, TestBuildGeometryFlag)
{
    EXPECT_EQ(true, m_sphere.getBuildGeometry());
}