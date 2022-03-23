#include <halberd/meta/type_list.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits> // std::integral_constant


namespace
{
    namespace ns = halberd::meta;

    template<int I>
    using int_tag = std::integral_constant<int, I>;
    using int_tag_end = int_tag<-1>;

    template<int IFind>
    struct int_tag_finder
    {
        template<int I>
        constexpr bool operator()(ns::type_wrapper<int_tag<I>>) const noexcept
        {
            return I == IFind;
        }
    };

    template<int IFind>
    constexpr int_tag_finder<IFind> int_tag_finder_v;

    template<int IRemove>
    struct int_tag_remover
    {
        template<int I>
        constexpr bool operator()(ns::type_wrapper<int_tag<I>>) const noexcept
        {
            return I == IRemove;
        }
    };

    template<int IRemove>
    constexpr int_tag_remover<IRemove> int_tag_remover_v;
}

TEST(type_list, find_if_empty)
{
    constexpr auto finder = int_tag_finder_v<0>;
    constexpr auto find_result = ns::find_if(ns::type_list_v<>, ns::type_wrapper_v<int_tag_end>, finder);

    ASSERT_EQ(int_tag_end::value, ns::unwrap(find_result));
}

TEST(type_list, find_if_single_type)
{
    constexpr auto finder = int_tag_finder_v<0>;
    constexpr auto find_result = ns::find_if(ns::type_list_v<int_tag<0>>, ns::type_wrapper_v<int_tag_end>, finder);

    ASSERT_EQ(0, ns::unwrap(find_result));
}

TEST(type_list, find_if_single_type_end)
{
    constexpr auto finder = int_tag_finder_v<1>;
    constexpr auto find_result = ns::find_if(ns::type_list_v<int_tag<0>>, ns::type_wrapper_v<int_tag_end>, finder);

    ASSERT_EQ(int_tag_end::value, ns::unwrap(find_result));
}

TEST(type_list, find_if_multi_types)
{
    constexpr auto finder = int_tag_finder_v<2>;
    constexpr auto find_result = ns::find_if(ns::type_list_v<int_tag<0>, int_tag<1>, int_tag<2>>, ns::type_wrapper_v<int_tag_end>, finder);

    ASSERT_EQ(2, ns::unwrap(find_result));
}

TEST(type_list, find_if_multi_types_end)
{
    constexpr auto finder = int_tag_finder_v<3>;
    constexpr auto find_result = ns::find_if(ns::type_list_v<int_tag<0>, int_tag<1>, int_tag<2>>, ns::type_wrapper_v<int_tag_end>, finder);

    ASSERT_EQ(int_tag_end::value, ns::unwrap(find_result));
}

TEST(type_list, remove_if_empty)
{
    constexpr auto remover = int_tag_remover_v<0>;
    constexpr auto remover_result = ns::remove_if(ns::type_list_v<>, remover);

    ASSERT_TRUE(ns::empty(remover_result));
}

TEST(type_list, remove_if_single_type)
{
    constexpr auto remover = int_tag_remover_v<0>;
    constexpr auto remover_result = ns::remove_if(ns::type_list_v<int_tag<0>>, remover);

    ASSERT_TRUE(ns::equals(ns::type_list_v<>, remover_result));
}

TEST(type_list, remove_if_single_type_no_match)
{
    constexpr auto remover = int_tag_remover_v<1>;
    constexpr auto remover_result = ns::remove_if(ns::type_list_v<int_tag<0>>, remover);

    ASSERT_TRUE(ns::equals(ns::type_list_v<int_tag<0>>, remover_result));
}

TEST(type_list, remove_if_multi_types)
{
    constexpr auto remover = int_tag_remover_v<2>;
    constexpr auto remover_result = ns::remove_if(ns::type_list_v<int_tag<0>, int_tag<1>, int_tag<2>>, remover);

    ASSERT_TRUE(ns::equals(ns::type_list_v<int_tag<0>, int_tag<1>>, remover_result));
}

TEST(type_list, remove_if_multi_types_no_match)
{
    constexpr auto remover = int_tag_remover_v<3>;
    constexpr auto remover_result = ns::remove_if(ns::type_list_v<int_tag<0>, int_tag<1>, int_tag<2>>, remover);

    ASSERT_TRUE(ns::equals(ns::type_list_v<int_tag<0>, int_tag<1>, int_tag<2>>, remover_result));
}
