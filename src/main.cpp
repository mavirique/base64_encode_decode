#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <expected>
#include <functional> // For std::visit
#include <span>       // For std::span and std::as_bytes

#include "base64.hpp"
#include "cli_parser.hpp"

// This function contains the actual "business logic" for each command
std::expected<void, std::string> perform_command(const EncodeCommand& cmd) {
    // FIX: Explicitly convert the string to a span of bytes for the encode function.
    // std::as_bytes creates a view over the raw data of the string.
    std::cout << "Encoded: " << base64::encode(std::as_bytes(std::span{cmd.input})) << '\n';
    return {};
}

std::expected<void, std::string> perform_command(const DecodeCommand& cmd) {
    auto decoded_result = base64::decode(cmd.input);
    if (!decoded_result) {
        return std::unexpected("Error: Invalid Base64 input string.");
    }
    
    // FIX: Convert the vector of bytes back into a string for printing.
    // This assumes the decoded data is valid text, which is typical for this kind of tool.
    std::string decoded_string(reinterpret_cast<const char*>(decoded_result->data()), decoded_result->size());
    std::cout << "Decoded: " << decoded_string << '\n';

    return {};
}

// Overload for handling the "help" or "usage" case.
void perform_command(const OperationMode& mode) {
    if (mode == OperationMode::Help) {
        print_usage(std::cout);
    }
}


int main(int argc, char* argv[]) {
    // Convert C-style args to a vector of string_views for safety and efficiency
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    auto res = parse_cli(args);
    if (!res) {
        std::cerr << res.error() << "\n\n";
        print_usage(std::cerr);
        return 1;
    }

    // Use std::visit for clean, type-safe dispatching of the command variant.
    int exit_code = std::visit([&](auto&& cmd) -> int {
        // Use an immediately-invoked lambda expression (IILE) to handle return types
        if constexpr (std::is_same_v<std::decay_t<decltype(cmd)>, OperationMode>) {
            perform_command(cmd);
            return 0;
        } else {
            auto outcome = perform_command(cmd);
            if (!outcome) {
                std::cerr << outcome.error() << '\n';
                return 1; // Return error code
            }
            return 0; // Return success code
        }
    }, res.value());
    
    return exit_code;
}