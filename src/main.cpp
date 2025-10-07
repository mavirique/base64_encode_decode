#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <expected>

#include "base64.hpp"
#include "cli_parser.hpp"

// This function contains the actual "business logic" for each command
std::expected<void, std::string> perform_command(const EncodeCommand& cmd) {
    std::cout << "Encoded: " << base64::encode(cmd.input) << '\n';
    return {};
}

std::expected<void, std::string> perform_command(const DecodeCommand& cmd) {
    // 1. Call decode and check the optional result
    auto decoded_result = base64::decode(cmd.input);
    
    if (!decoded_result) {
        // If it's empty, the input was invalid. Return an error.
        return std::unexpected("Error: Invalid Base64 input string.");
    }
    
    // 2. If it's valid, print the decoded string
    std::cout << "Decoded: " << *decoded_result << '\n';
    
    // 3. Return successfully
    return {};
}

int main(int argc, char* argv[]) {
    // Convert C-style args to a vector of string_views
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    // Call the parser
    auto res = parse_cli(args);
    if (!res) {
        std::cerr << res.error() << "\n\n";
        print_usage(std::cerr);
        return 1;
    }

    // Handle the variant's result
    if (std::holds_alternative<OperationMode>(res.value())) {
        print_usage(std::cout);
        return 0;
    }

    // Create a generic dispatcher lambda to execute commands
    auto dispatch = [&](auto&& cmd) {
        auto outcome = perform_command(cmd);
        if (!outcome) {
            // The dispatcher now handles errors from both encoding and decoding
            std::cerr << outcome.error() << '\n';
            return 1;
        }
        return 0;
    };

    // Check which command type is in the variant and dispatch it
    if (std::holds_alternative<EncodeCommand>(res.value())) {
        return dispatch(std::get<EncodeCommand>(res.value()));
    } else {
        return dispatch(std::get<DecodeCommand>(res.value()));
    }
}