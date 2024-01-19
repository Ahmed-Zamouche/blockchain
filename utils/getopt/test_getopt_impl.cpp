#include "getopt_impl.h"
#include <gtest/gtest.h>
#include <cstddef>
#include <iomanip>
#include <vector>

struct Expected
{
    explicit Expected(int optind_, const char* optarg_, int retval_) : optind{optind_}, optarg{optarg_}, retval{retval_}
    {
    }
    int opterr{};
    int optopt{};
    int optind{};
    const char* optarg{};
    int retval{};
};
struct TestParam
{
    int argc;
    std::vector<const char*> argv;
    const char* optstring;
    std::vector<Expected> expected;
};

std::ostream& operator<<(std::ostream& os, const TestParam& param)
{
    os << "{\"argc\": " << param.argc << ", \"argv\": [";
    for (auto&& arg : param.argv)
    {
        if (arg)
            os << std::quoted(arg);
        else
            os << "null";
        os << ", ";
    }
    os << "], "
       << " \"optstring\": " << std::quoted(param.optstring) << ", \"expected\": [";

    for (auto&& expected : param.expected)
    {
        if (expected.retval == -1)
            os << expected.retval;
        else
            os << "\"" << (char)expected.retval << "\"";
    }
    os << "]";
    os << "}";
    return os;
}

class GetOptParamFixtureTest : public ::testing::TestWithParam<TestParam>
{
    void SetUp() override
    {
        opterr = -1;
        optind = 1;
        optarg = NULL;
    }
    void TearDown() override {}
};

TEST_P(GetOptParamFixtureTest, TestCase)
{
    auto& param = GetParam();
    int retval{};
    size_t i = 0;
    while ((retval = getopt(param.argc, (char* const*)param.argv.data(), param.optstring)) != -1)
    {
        EXPECT_EQ((char)retval, (char)param.expected[i].retval);
        i++;
    }
}

INSTANTIATE_TEST_SUITE_P(
    GetOptTest, GetOptParamFixtureTest,
    ::testing::Values(
        // Test type

        TestParam{1, {"cmd"}, "", {Expected{1, NULL, -1}}}, TestParam{2, {"cmd", "a"}, "a", {Expected{1, NULL, -1}}},
        TestParam{2, {"cmd", "-"}, "a", {Expected{1, NULL, -1}}},
        TestParam{2, {"cmd", "-x"}, "a", {Expected{1, NULL, '?'}}},
        TestParam{2, {"cmd", "-x"}, ":a", {Expected{1, NULL, '?'}}},
        TestParam{2, {"cmd", "-a"}, "a:", {Expected{1, NULL, '?'}}},
        TestParam{2, {"cmd", "-a"}, ":a:", {Expected{1, NULL, ':'}}},
        TestParam{2, {"cmd", "-aarg0"}, "a:", {Expected{1, "arg0", 'a'}}},
        TestParam{2, {"cmd", "-aarg1"}, ":a:", {Expected{1, "arg1", 'a'}}},
        TestParam{3, {"cmd", "-a", "arg0"}, ":a:", {Expected{1, "arg0", 'a'}}},
        TestParam{3, {"cmd", "-a", "arg1"}, ":a:", {Expected{1, "arg1", 'a'}}},
        TestParam{2, {"cmd", "--"}, "", {Expected{2, NULL, -1}}},
        TestParam{
            5, {"cmd", "-ao", "arg", "path", "path"}, ":abf:o:", {Expected{1, NULL, 'a'}, Expected{1, "arg", 'o'}}},
        TestParam{6,
                  {"cmd", "-a", "-o", "arg", "path", "path"},
                  ":abf:o:",
                  {Expected{1, NULL, 'a'}, Expected{1, "arg", 'o'}}},
        TestParam{6,
                  {"cmd", "-o", "arg", "-a", "path", "path"},
                  ":abf:o:",
                  {Expected{1, "arg", 'o'}, Expected{1, NULL, 'a'}}},
        TestParam{7,
                  {"cmd", "-a", "-o", "arg", "--", "path", "path"},
                  ":abf:o:",
                  {Expected{1, NULL, 'a'}, Expected{1, "arg", 'o'}}},
        TestParam{
            5, {"cmd", "-a", "-oarg", "path", "path"}, ":abf:o:", {Expected{1, NULL, 'a'}, Expected{1, "arg", 'o'}}},
        TestParam{4, {"cmd", "-aoarg", "path", "path"}, ":abf:o:", {Expected{1, NULL, 'a'}, Expected{1, "arg", 'o'}}}));
