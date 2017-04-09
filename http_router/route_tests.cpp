#include <unordered_map>
#include "gtest/gtest.h"
#include "my_router.h"
#include "route_tests.h"
using namespace std;

TEST(MyRouterTests, SuccessWay) {

    MyRouter router;

    ASSERT_EQ(router.add_rule("/posts/new"), 0);
    ASSERT_EQ(router.add_rule("/posts/:id"), 1);
    ASSERT_EQ(router.add_rule("/posts/:id/delete"), 2);
    ASSERT_EQ(router.add_rule("/blogs/:name/posts/:id"), 3);

    ASSERT_EQ(router.get_rule_by_id(3), "/blogs/:name/posts/:id/");

    std::unordered_map<std::string, std::string> param;
    param.clear();
    ASSERT_EQ(router.find_request("/posts/1", &param), 1);
    ASSERT_EQ(param["id"], "1");

    ASSERT_EQ(router.find_request("/posts/super-mega-long-title", &param), 1);
    ASSERT_EQ(param["id"], "super-mega-long-title");

    ASSERT_EQ(router.find_request("/posts/new"), 0);

    ASSERT_EQ(router.find_request("/blogs/petya/posts/999-abc", &param), 3);
    ASSERT_EQ(param["name"], "petya");
    ASSERT_EQ(param["id"], "999-abc");
}


TEST(MyRouterTests, FailedWay) {

    MyRouter router;

    ASSERT_EQ(router.add_rule("/posts/new"), 0);
    ASSERT_EQ(router.add_rule("/posts/:id"), 1);
    ASSERT_EQ(router.add_rule("/posts/:id/delete"), 2);
    ASSERT_EQ(router.add_rule("/blogs/:name/posts/:id"), 3);

    ASSERT_NE(router.get_rule_by_id(5), "/blogs/:name/posts/:id/");

    std::unordered_map<std::string, std::string> param;
    param.clear();
    ASSERT_NE(router.find_request("/p0sts/1", &param), 1);
    ASSERT_NE(param["id"], "1");

    ASSERT_NE(router.find_request("/lol/kek-mega-long-title", &param), 1);
    ASSERT_NE(param["id"], "super-mega-long-title");

    ASSERT_NE(router.find_request("/posts/new/post"), 0);

    ASSERT_NE(router.find_request("/blogi/petya/posts/999-abc", &param), 3);
    ASSERT_NE(param["name"], "petya");
    ASSERT_NE(param["id"], "999-abc");
}

