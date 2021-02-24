#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

/**
 * Print out the usage information for this application.
 *
 * @param argv0 A string for the way that this application was
 * invoked on the command line (e.g., "./ziplines").
 */
void usage(const std::string &argv0) {
  std::cerr << argv0 << " <delimiter> <file1> ... <fileN>" << std::endl;
}

// An offset into ARGC where the list of filenames begins.
// ./ziplines , file1 file2 file3
//                ^
//                2
//        ARGC_FILENAME_OFFSET
const unsigned int ARGC_FILENAME_OFFSET = 2;

int main(int argc, char **argv) {
  // Check the number of parameters given to the application.
  // If there are fewer than 3, print the usage and quit.
  if (argc < 3) {
    usage(argv[0]);
    return 1;
  }

  // The delimiter to place between the lines from different files.
  std::string delimiter{argv[1]};
  // While building the zipped-line file, use `lines`, a vector of string
  // vectors.
  std::vector<std::vector<std::string>> lines{};
  // The length of the longest file, in lines. It starts at 0, obviously.
  size_t longest_file_linecount = 0;

  for (int filename_it = ARGC_FILENAME_OFFSET; filename_it < argc;
       filename_it++) {
    // The name of the file being zipped in.
    std::string filename{argv[filename_it]};
    // The current line of `filename` being zipped in.
    std::string line{};
    // The number of the current line of `filename` being zipped in.
    unsigned int current_line_number = 0;
    // Open `filename` for reading.
    std::ifstream input_fs{filename, std::ifstream::in};

    // For each of the lines in the file ...
    while (std::getline(input_fs, line)) {
      // Always make sure that there is enough space in the vector
      // that stores the zipped lines.
      if (current_line_number >= longest_file_linecount) {
        longest_file_linecount = current_line_number + 1;
        lines.resize(longest_file_linecount);

        // In order to keep everything aligned, we need to pre-populate
        // this line with empty lines from files that have been previously
        // zipped in. The goal is to maintain alignment.
        // We want
        // Line1 File1 <delimiter> Line1 File2 <delimiter> Line1 File3
        // Line2 File1 <delimiter> Line2 File2 <delimiter> Line2 File3
        //             <delimiter>             <delimiter> Line1 File3
        // and not
        // Line1 File1 <delimiter> Line1 File2 <delimiter> Line1 File3
        // Line2 File1 <delimiter> Line2 File2 <delimiter> Line2 File3
        // Line1 File3
        // for instance.
        for (int empty_line_it = 0; empty_line_it < filename_it - ARGC_FILENAME_OFFSET; empty_line_it++) {
          lines[current_line_number].push_back("");
        }
      }

      // Simple case: Just add the file!
      lines[current_line_number].push_back(line);
      current_line_number++;
    }

    // If this file didn't make it to the longest_file_linecount, then we
    // need to add some empty lines to aid alignment.
    // We want
    // Line1 File1 <delimiter> Line1 File2
    // Line2 File1 <delimiter> Line2 File2
    // Line3 File1 <delimiter>             <delimiter>
    // and not
    // Line1 File1 <delimiter> Line1 File2 <delimiter> Line1 File3
    // Line2 File1 <delimiter> Line2 File2 <delimiter> Line2 File3
    // Line3 File1 <delimiter> Line1 File3
    // for instance.
    for (int empty_line_it = 0; empty_line_it < longest_file_linecount - current_line_number; empty_line_it++) {
      lines[empty_line_it + current_line_number].push_back("");
    }
  }

  // Each of the lines is currently a vector of strings. Turn each line into
  // a string with each of the elements separated by `delimiter`. Shamelessly
  // stolen from
  // https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
  for (auto linevec : lines) {
    std::ostringstream combined_line{""};
    std::copy(
        linevec.begin(), linevec.end(),
        std::ostream_iterator<std::string>(combined_line, delimiter.c_str()));
    std::cout << combined_line.str() << std::endl;
  }
}
