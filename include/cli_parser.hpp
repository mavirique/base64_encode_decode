#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <expected>
#include <variant>
#include <iosfwd>

// Command structures
struct EncodeCommand {
    std::string input;
};

struct DecodeCommand {
    std::string input;
};

// Operation mode
enum class OperationMode { Help };

// Variant to hold any possible command
using CommandVariant = std::variant<EncodeCommand, DecodeCommand, OperationMode>;

/**
 * @brief Parses command-line arguments.
 * @param args A vector of string_views representing the arguments.
 * @return An std::expected containing either a CommandVariant on success or an error string
 */
std::expected<CommandVariant, std::string> parse_cli(const std::vector<std::string_view>& args);

/**
 * @brief Prints the usage information to the provided output stream.
 * @param outputstream The stream to write the usage information to.
 */
void print_usage(std::ostream& outputstream);