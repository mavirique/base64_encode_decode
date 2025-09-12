#include <vector>
#include <string>
#include <string_view>
#include <span>
#include <optional>
#include <cstddef>

// Encodes a span of bytes into a Base64 string.
std::string base64_encode(std::span<const std::byte> data);

// Decodes a Base64 string_view into a vector of byes.
// Returns std::nullopt if the input is malformed.
std::optional<std::vector<std::byte>> base64_decode(std::string_view data);