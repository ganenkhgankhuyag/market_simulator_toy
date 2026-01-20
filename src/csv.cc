#include "csv.h"

CSVWriter::CSVWriter(const std::string& path) : out_(path) {}

std::string CSVWriter::escape(const std::string& s) {
    bool needs_quotes = false;

    for (char c : s) {
        needs_quotes = needs_quotes ||
                       c == ',' || c == '"' || c == '\n' || c == '\r';
    }

    std::string result;

    if (!needs_quotes) {
        result = s;
    } else {
        result = "\"";
        for (char c : s) {
            if (c == '"') result += "\"\"";
            else result += c;
        }
        result += "\"";
    }

    return result;
}


void CSVWriter::write_header(const std::vector<std::string>& cols) {
    write_row(cols);
}

void CSVWriter::write_row(const std::vector<std::string>& cells) {
    for (size_t i = 0; i < cells.size(); i++) {
        if (i) out_ << ",";
        out_ << escape(cells[i]);
    }
    out_ << "\n";
}
