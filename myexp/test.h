#ifndef DYB_TEST
#define DYB_TEST

#include <string>
#include <iostream>

namespace dyb
{

    inline void test_assert(bool expression, const std::string & file, size_t line)
    {
        if (expression) return;
        std::cout << "test fail : " << file << " line " << line << std::endl;
    }

#define TEST_ASSERT(expression) \
    test_assert(expression, __FILE__, __LINE__);

}

#endif
