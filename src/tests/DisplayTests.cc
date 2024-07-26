#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Display.h"

TEST_CASE("Test Empty Display Constructor")
{
    Display emptyTestDisplay;
    CHECK(emptyTestDisplay.getWidth() == 600);
    CHECK(emptyTestDisplay.getHeight() == 400);
}

TEST_CASE("Test Display Constructor with parameters")
{
    Display testDisplay = Display(800, 600, 30);
    CHECK(testDisplay.getWidth() == 800);
    CHECK(testDisplay.getHeight() == 600);
}