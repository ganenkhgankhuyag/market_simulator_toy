#ifndef CSV_H
#define CSV_H

#include <fstream>
#include <string>
#include <vector>

class CSVWriter {

    // Purpose:
    //   Provides a minimal interface for writing rows to a CSV file.
    //   Handles basic CSV escaping so each row is written correctly.

public:

    // PRE: path is a valid file path (the parent directory should exist).
    // POST: Attempts to open the file at path for writing. If opening fails,
    //       good() will be false and writes will not succeed.
    CSVWriter(const std::string& path);

    // PRE: cols is defined and contains the column names you want in the CSV.
    // POST: Writes one CSV line containing these column names, separated by commas.
    //       This should be called once at the start of writing a new CSV file.
    void write_header(const std::vector<std::string>& cols);

    // PRE: cells is defined and contains the values for one row, as strings.
    // POST: Writes one CSV line containing these cells, separated by commas.
    //       Each cell is escaped when needed so the CSV remains valid.
    void write_row(const std::vector<std::string>& cells);

    // POST: returns true if the output stream is in a good state (file opened and
    //       no write errors so far). Used to detect file/open/write failures.
    bool good() const { return m_out.good(); }

private:

    // Output file stream for the CSV being written.
    std::ofstream m_out;

    // PRE: s is a single cell value (not a whole row).
    // POST: returns a CSV-safe version of s, such that:
    //       - if s contains a comma, quote, or newline, it is wrapped in quotes
    //       - any internal quotes are doubled (" -> ""),
    //       preventing a single cell from accidentally splitting into multiple columns.
    std::string escape(const std::string& s);
};

#endif // CSV_H
