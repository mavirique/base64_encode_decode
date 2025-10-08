#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <cstddef> // For std::byte
#include <span>      // For std::span

namespace base64 {

    /**
     * @brief Encodes a span of bytes into a Base64 string.
     * * @param data The raw binary data to encode.
     * @return The resulting Base64 encoded string.
     */
    std::string encode(std::span<const std::byte> data);
    
    /**
     * @brief Decodes a Base64 string into a vector of bytes.
     * * This implementation is hardened against timing attacks.
     * * @param data The Base64 encoded string to decode.
     * @return A std::vector<std::byte> on success, or std::nullopt if the input is malformed.
     */
    std::optional<std::vector<std::byte>> decode(std::string_view data);
}