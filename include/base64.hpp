#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <cstddef>

namespace base64 {

    // Encodes a span of bytes into a Base64 string.
    std::string encode(std::string_view str);
    
    // Decodes a Base64 string_view into a vector of byes.
    // Returns std::nullopt if the input is malformed.
    std::optional<std::string> decode(std::string_view data);
}
