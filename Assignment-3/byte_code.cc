#include "byte_code.hh"

namespace byte_code {
    bool is_number(const std::string& str) {
        try {
            std::size_t pos;
            std::stoi(str, &pos);
            return pos == str.size(); // Check if the entire string was consumed
        } catch (std::invalid_argument const &e) {
            return false;
        } catch (std::out_of_range const &e) {
            return false;
        }
    }
}