// This tells Catch2 to provide a main() function for this test executable
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "base64.hpp" // The library we are testing
#include <string>

// Helper to convert a string to a vector of bytes for testing
std::vector<std::byte> to_bytes(const std::string& str) {
    std::vector<std::byte> bytes;
    bytes.resize(str.size());
    std::transform(str.begin(), str.end(), bytes.begin(), [](char c) {
        return static_cast<std::byte>(c);
    });
    return bytes;
}

TEST_CASE("Base64 Encoding", "[base64]") {
    SECTION("No Padding") {
        REQUIRE(base64_encode(to_bytes("Man")) == "TWFu");
    }
    SECTION("One Padding Character") {
        REQUIRE(base64_encode(to_bytes("light work.")) == "bGlnaHQgd29yay4=");
    }
    SECTION("Two Padding Characters") {
        REQUIRE(base64_encode(to_bytes("Hello")) == "SGVsbG8=");
    }
    SECTION("Empty String") {
        REQUIRE(base64_encode(to_bytes("")) == "");
    }
}

TEST_CASE("Base64 Decoding", "[base64]") {
    SECTION("No Padding") {
        auto decoded = base64_decode("TWFu");
        REQUIRE(decoded.has_value());
        REQUIRE(decoded->size() == 3);
        REQUIRE(static_cast<char>((*decoded)[0]) == 'M');
    }
    SECTION("With Padding") {
        auto decoded = base64_decode("SGVsbG8=");
        REQUIRE(decoded.has_value());
        REQUIRE(decoded->size() == 5);
        REQUIRE(std::string_view(reinterpret_cast<const char*>(decoded->data()), decoded->size()) == "Hello");
    }
    SECTION("Empty String") {
        auto decoded = base64_decode("");
        REQUIRE(decoded.has_value());
        REQUIRE(decoded->empty());
    }
    SECTION("Invalid Length") {
        REQUIRE_FALSE(base64_decode("SGVsbG8").has_value()); // Missing padding
        REQUIRE_FALSE(base64_decode("SGV").has_value());     // Length not multiple of 4
    }
    SECTION("Invalid Characters") {
        REQUIRE_FALSE(base64_decode("TWF#").has_value());
    }
}