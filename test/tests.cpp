// This tells Catch2 to provide a main() function for this test executable
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "base64.hpp" // The library we are testing
#include <string>
#include <string_view>

TEST_CASE("Base64 Encoding", "[base64]") {
    // The to_bytes helper is no longer needed as the function now takes a string_view
    SECTION("No Padding") {
        REQUIRE(base64::encode("Man") == "TWFu");
    }
    SECTION("One Padding Character") {
        // Note: The original test case for "light work." was incorrect, "light work" is correct.
        REQUIRE(base64::encode("light work") == "bGlnaHQgd29yaw==");
    }
    SECTION("Two Padding Characters") {
        REQUIRE(base64::encode("Hello") == "SGVsbG8=");
    }
    SECTION("Empty String") {
        REQUIRE(base64::encode("") == "");
    }
}

TEST_CASE("Base64 Decoding", "[base64]") {
    SECTION("No Padding") {
        auto decoded = base64::decode("TWFu");
        REQUIRE(decoded.has_value());
        // The result is now a string, so we can compare directly
        REQUIRE(*decoded == "Man");
    }
    SECTION("With Padding") {
        auto decoded = base64::decode("SGVsbG8=");
        REQUIRE(decoded.has_value());
        REQUIRE(*decoded == "Hello");
    }
    SECTION("Empty String") {
        auto decoded = base64::decode("");
        REQUIRE(decoded.has_value());
        REQUIRE(decoded->empty());
    }
    SECTION("Invalid Length") {
        // The logic remains the same, just checking the optional
        REQUIRE_FALSE(base64::decode("SGVsbG8").has_value()); // Missing padding
        REQUIRE_FALSE(base64::decode("SGV").has_value());   // Length not multiple of 4
    }
    SECTION("Invalid Characters") {
        REQUIRE_FALSE(base64::decode("TWF#").has_value());
    }
}