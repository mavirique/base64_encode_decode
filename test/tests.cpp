#include "base64.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <span>

// Helper function to convert a std::vector<std::byte> to a std::string
// This makes the tests cleaner.
std::string to_string(const std::vector<std::byte>& bytes) {
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}


TEST_CASE("Base64 Encoding", "[base64]") {
    SECTION("Encoding simple strings") {
        // FIX: Use a std::string_view to create a span, then convert to bytes.
        using namespace std::string_view_literals;
        REQUIRE(base64::encode(std::as_bytes(std::span{"Man"sv})) == "TWFu");
    }

    SECTION("Encoding with padding") {
        using namespace std::string_view_literals;
        REQUIRE(base64::encode(std::as_bytes(std::span{"light work"sv})) == "bGlnaHQgd29yaw==");
        REQUIRE(base64::encode(std::as_bytes(std::span{"Hello"sv})) == "SGVsbG8=");
    }

    SECTION("Encoding an empty string") {
        using namespace std::string_view_literals;
        REQUIRE(base64::encode(std::as_bytes(std::span{""sv})) == "");
    }
}

TEST_CASE("Base64 Decoding", "[base64]") {
    SECTION("Decoding simple strings") {
        auto decoded = base64::decode("TWFu");
        REQUIRE(decoded.has_value());
        // FIX: Convert the resulting vector of bytes to a string for comparison.
        REQUIRE(to_string(*decoded) == "Man");
    }

    SECTION("Decoding with padding") {
        auto decoded = base64::decode("SGVsbG8=");
        REQUIRE(decoded.has_value());
        REQUIRE(to_string(*decoded) == "Hello");
    }

    SECTION("Decoding an empty string") {
        auto decoded = base64::decode("");
        REQUIRE(decoded.has_value());
        REQUIRE(to_string(*decoded) == "");
    }

    SECTION("Decoding invalid characters") {
        // Invalid characters should result in std::nullopt
        REQUIRE_FALSE(base64::decode("invalid$").has_value());
        REQUIRE_FALSE(base64::decode("TWFu?").has_value());
    }

    SECTION("Decoding with incorrect padding") {
        // Incorrect padding should result in failure.
        REQUIRE_FALSE(base64::decode("SGVsbG8").has_value()); // Missing '='

        // FIX: Replace the incorrect test case with a truly invalid one.
        // "SGVsbA==" is the valid encoding for "Hell".
        // "SGVsbC==" is INVALID because 'C' (value 2) has non-zero bits (000010)
        // where padding rules require zeros for the final sextet.
        REQUIRE_FALSE(base64::decode("SGVsbC==").has_value());
    }
}