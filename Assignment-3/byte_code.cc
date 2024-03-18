#include "byte_code.hh"

namespace byte_code {
    bool is_number(const std::string& str) { // Check if the string is a number
        try { // Try to convert the string to a number
            std::size_t pos; // Position of the first non-numeric character
            std::stoi(str, &pos); // Convert the string to a number
            return pos == str.size(); // Check if the entire string was consumed
        } catch (std::invalid_argument const &e) { // Catch invalid argument exception
            return false;
        } catch (std::out_of_range const &e) { // Catch out of range exception
            return false;
        }
    }
}