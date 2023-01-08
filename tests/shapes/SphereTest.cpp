#include <gtest/gtest.h>
#include "sphere.h"

class SphereTest : public testing::Test 
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

    testing::AssertionResult IsBuildGeometryFlagSet()
    {
        if(m_sphere.getBuildGeometry())
        {
            return testing::AssertionSuccess();
        }
        else 
        {
            return testing::AssertionFailure() << "vertices won't be generated for rasterization";
        }
    }

    Sphere m_sphere;
};

TEST_F(SphereTest, TestRadius)
{
    EXPECT_EQ(5.1f, m_sphere.getRadius());
}

TEST_F(SphereTest, TestBuildGeometryFlag)
{
    EXPECT_TRUE(IsBuildGeometryFlagSet());
}