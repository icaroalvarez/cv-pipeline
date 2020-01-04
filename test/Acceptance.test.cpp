#include "trompeloeil.hpp"
#include "catch2/catch.hpp"

SCENARIO("An image processor pipeline can be loaded")
{
    GIVEN("A pipeline definition with non existing image processors")
    {
        WHEN("Loading the pipeline is requested")
        {
            THEN("It fails loading")
            {

            }
        }
    }

    GIVEN("A pipeline definition with all existing image processors")
    {
        WHEN("Loading the pipeline is requested")
        {
            THEN("It loads successfully")
            {

            }
        }
    }
}

SCENARIO("A pipeline's image processor can be configured")
{
    GIVEN("A pipeline already loaded")
    {
        THEN("It fails if the Image processor is configured with incorrect parameters")
        {

        }

        THEN("Doesn't fail if the Image processor is configured with correct parameters")
        {

        }
    }

}

SCENARIO("An image from frame source can be processed through the pipeline")
{
    GIVEN("A pipeline already loaded")
    {
        WHEN("An image is fired to be processed")
        {
            THEN("The observer is notified when the image is processed by all image processors")
            {
                AND_THEN("The original, pre-processed, post-processed and debug images can be retrieved from an image processor")
                {

                }
            }
        }
    }
}