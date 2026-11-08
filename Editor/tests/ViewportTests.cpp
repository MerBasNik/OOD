//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "View/Viewport/Viewport.h"
#include <gtest/gtest.h>

class ViewportTest : public ::testing::Test
{
public:
	Viewport m_viewport{ { 1000, 600 }, { 1000, 600 }, 0.25, 8 };

	static void ExpectPointNear(const Point actual, const Point expected)
	{
		EXPECT_NEAR(actual.m_x, expected.m_x, 1e-9);
		EXPECT_NEAR(actual.m_y, expected.m_y, 1e-9);
	}
};

TEST_F(ViewportTest, InitialViewShowsContentOneToOne)
{
	EXPECT_EQ(m_viewport.GetZoom(), 1);
	ExpectPointNear(m_viewport.ToContent({ 250, 100 }), { 250, 100 });
	ExpectPointNear(m_viewport.ToView({ 250, 100 }), { 250, 100 });
}

TEST_F(ViewportTest, ZoomAtKeepsContentPointUnderCursor)
{
	const Point before = m_viewport.ToContent({ 300, 200 });

	m_viewport.ZoomAt(2, { 300, 200 });

	EXPECT_EQ(m_viewport.GetZoom(), 2);
	ExpectPointNear(m_viewport.ToContent({ 300, 200 }), before);
}

TEST_F(ViewportTest, ToViewIsInverseOfToContent)
{
	m_viewport.ZoomAt(3, { 700, 100 });

	ExpectPointNear(m_viewport.ToView(m_viewport.ToContent({ 123, 456 })), { 123, 456 });
}

TEST_F(ViewportTest, ZoomIsClampedToLimits)
{
	m_viewport.ZoomAt(100, { 0, 0 });
	EXPECT_EQ(m_viewport.GetZoom(), 8);

	m_viewport.ZoomAt(0.001, { 0, 0 });
	EXPECT_EQ(m_viewport.GetZoom(), 0.25);
}

TEST_F(ViewportTest, ZoomInAndOutUseViewCenter)
{
	const Point center = m_viewport.ToContent({ 500, 300 });

	m_viewport.ZoomIn();
	EXPECT_EQ(m_viewport.GetZoom(), Viewport::ZOOM_STEP);
	ExpectPointNear(m_viewport.ToContent({ 500, 300 }), center);

	m_viewport.ZoomOut();
	EXPECT_NEAR(m_viewport.GetZoom(), 1, 1e-9);
}

TEST_F(ViewportTest, ScrollMovesVisibleAreaWhenZoomedIn)
{
	m_viewport.ZoomAt(2, { 0, 0 });

	m_viewport.Scroll({ 100, 50 });

	ExpectPointNear(m_viewport.GetOffset(), { 50, 25 });
}

TEST_F(ViewportTest, ScrollIsClampedToContent)
{
	m_viewport.ZoomAt(2, { 0, 0 });

	m_viewport.Scroll({ 10000, 10000 });
	ExpectPointNear(m_viewport.GetOffset(), { 500, 300 });

	m_viewport.Scroll({ -10000, -10000 });
	ExpectPointNear(m_viewport.GetOffset(), { 0, 0 });
}

TEST_F(ViewportTest, ScrollDoesNothingWhenContentFits)
{
	m_viewport.Scroll({ 100, 100 });

	ExpectPointNear(m_viewport.GetOffset(), { 0, 0 });
}

TEST_F(ViewportTest, ZoomedOutContentIsCentered)
{
	m_viewport.ZoomAt(0.5, { 0, 0 });

	ExpectPointNear(m_viewport.GetOffset(), { -500, -300 });
	ExpectPointNear(m_viewport.ToView({ 0, 0 }), { 250, 150 });
}

TEST_F(ViewportTest, ResetZoomRestoresInitialView)
{
	m_viewport.ZoomAt(4, { 800, 500 });

	m_viewport.ResetZoom();

	EXPECT_EQ(m_viewport.GetZoom(), 1);
	ExpectPointNear(m_viewport.GetOffset(), { 0, 0 });
}

TEST_F(ViewportTest, InvalidZoomLimitsAreRejected)
{
	EXPECT_THROW(Viewport({ 10, 10 }, { 10, 10 }, 0, 2), std::invalid_argument);
	EXPECT_THROW(Viewport({ 10, 10 }, { 10, 10 }, 2, 4), std::invalid_argument);
	EXPECT_THROW(Viewport({ 10, 10 }, { 10, 10 }, 0.5, 0.8), std::invalid_argument);
}
