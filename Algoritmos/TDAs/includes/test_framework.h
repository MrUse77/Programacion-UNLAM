#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

// Colores para output
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

// Contadores globales
static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

// Macros para tests
#define TEST(name) void test_##name()

#define RUN_TEST(name) do { \
    printf("\n[TEST] %s...\n", #name); \
    tests_total++; \
    test_##name(); \
} while(0)

#define ASSERT(condition, message) do { \
    if (!(condition)) { \
        printf(COLOR_RED "  ✗ FAILED: %s\n" COLOR_RESET, message); \
        printf("    Expected: %s\n", #condition); \
        printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
        tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_EQUAL(expected, actual, message) do { \
    if ((expected) != (actual)) { \
        printf(COLOR_RED "  ✗ FAILED: %s\n" COLOR_RESET, message); \
        printf("    Expected: %d\n", (int)(expected)); \
        printf("    Actual:   %d\n", (int)(actual)); \
        printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
        tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_TRUE(condition, message) ASSERT((condition), message)
#define ASSERT_FALSE(condition, message) ASSERT(!(condition), message)

#define TEST_PASSED(message) do { \
    printf(COLOR_GREEN "  ✓ PASSED: %s\n" COLOR_RESET, message); \
    tests_passed++; \
} while(0)

#define TEST_SUMMARY() do { \
    printf("\n" COLOR_YELLOW "========================================\n"); \
    printf("TEST SUMMARY\n"); \
    printf("========================================\n" COLOR_RESET); \
    printf("Total:  %d\n", tests_total); \
    printf(COLOR_GREEN "Passed: %d\n" COLOR_RESET, tests_passed); \
    if (tests_failed > 0) { \
        printf(COLOR_RED "Failed: %d\n" COLOR_RESET, tests_failed); \
    } else { \
        printf("Failed: 0\n"); \
    } \
    printf(COLOR_YELLOW "========================================\n" COLOR_RESET); \
    return tests_failed; \
} while(0)

#endif // TEST_FRAMEWORK_H
