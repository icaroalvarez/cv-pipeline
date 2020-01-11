#include <catch2/catch.hpp>
#include <Factory.h>

SCENARIO("Factory makes registered objects")
{
    GIVEN("A MyType factory")
    {
        struct MyTypeBase {};
        struct MyTypeDerived : public MyTypeBase{};
        Factory<MyTypeBase> factory;

        WHEN("A MyTypeDerived is registered using the name they were registered")
        {
            constexpr auto myTypeName{"my_type_derived"};
            factory.registerMaker<MyTypeDerived>(myTypeName);

            THEN("Objects can be created")
            {
                const auto myTypeDerived{factory.make(myTypeName)};
                CHECK(typeid(myTypeDerived) == typeid(std::make_unique<MyTypeBase>()));
            }

            THEN("Objects creation fails if the name is not registered")
            {
                CHECK_THROWS_WITH(factory.make("not_registered_name"), "not_registered_name not registered");
            }
        }
    }
}