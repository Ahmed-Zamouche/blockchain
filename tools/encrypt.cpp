#include "utils/getopt/getopt_impl.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

enum class Operation {
  none,
  encrypt,
  decrypt,
};

struct Arguments {
  std::string infile;
  std::unique_ptr<std::istream> is;
  std::string outfile;
  std::unique_ptr<std::ostream> os;
  Operation opr;
  bool verbose;
};

std::unique_ptr<std::ostream> get_ostream(std::string &filename) {
  if (!filename.empty()) {
    std::unique_ptr<std::ostream> ofs = std::unique_ptr<std::ostream>(
        new std::ofstream{filename, std::ofstream::out | std::ofstream::binary |
                                        std::ofstream::trunc});
    if (!*ofs)
      return nullptr;
    return ofs;
  } else {
    filename = "stdin";
  }
  return std::unique_ptr<std::ostream>{&std::cout};
}

std::unique_ptr<std::istream> get_istream(std::string &filename) {
  if (!filename.empty()) {
    std::unique_ptr<std::istream> ifs = std::unique_ptr<std::istream>(
        new std::ifstream{filename, std::ofstream::in | std::ofstream::binary});
    if (!*ifs)
      return nullptr;
    return ifs;
  } else {
    filename = "stdout";
  }

  return std::unique_ptr<std::istream>{&std::cin};
}

void print_help(std::ostream &os, const char *cmd) {
  const char *bname = cmd + strlen(cmd);
  while (bname != cmd && *--bname != '/')
    ;
  bname++;

  os << "Usage: " << bname << " [OPTIONS]" << std::endl;
  os << "Options:" << std::endl;
  os << "  -i <infile> " << std::endl
     << "    set input file. stdin if not supplied" << std::endl;
  os << "  -o <outfile> " << std::endl
     << "    set output file. stdout if not supplied" << std::endl;
  os << "  -e" << std::endl << "    select encrypt" << std::endl;
  os << "  -d" << std::endl << "    select decrypt" << std::endl;
  os << "  -h" << std::endl << "    print this help and exit" << std::endl;
}
Arguments parse_args(int argc, char *argv[]) {
  int opt;
  Arguments args{};
  bool error = false;
  while ((opt = getopt(argc, argv, "hedi:o:")) != -1) {
    switch (opt) {
    case 'i':
      args.infile = optarg;
      break;
    case 'o':
      args.outfile = optarg;
      break;
    case 'e':
      if (args.opr == Operation::none)
        args.opr = Operation::encrypt;
      else
        error = true;
      break;
    case 'd':
      if (args.opr == Operation::none)
        args.opr = Operation::decrypt;
      else
        error = true;
      break;
    case 'h':
      print_help(std::cout, argv[0]);
      exit(EXIT_SUCCESS);
    default:
      error = true;
      break;
    }
  }

  if (error || args.opr == Operation::none) {
    print_help(std::cerr, argv[0]);
    exit(EXIT_FAILURE);
  }
  return args;
}

#include <iomanip>
void print_vector(std::ostream &os, const std::vector<uint8_t> &bytes) {
  for (auto &&b : bytes) {
    os << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(b);
  }
}
int main(int argc, char **argv) {
  auto args = parse_args(argc, argv);

  args.is = get_istream(args.infile);
  if (!args.is) {
    std::cerr << "Failed to open input stream" << std::endl;
    exit(EXIT_FAILURE);
  }

  args.os = get_ostream(args.outfile);
  if (!args.os) {
    std::cerr << "Failed to open output stream" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<uint8_t> in_data(std::istreambuf_iterator<char>(*args.is),
                               std::istreambuf_iterator<char>());
  if (!*args.is) {
    std::cerr << "Failed to read from the input stream" << std::endl;
    exit(EXIT_FAILURE);
  }
  static_cast<std::ifstream *>(args.is.get())->close();

  // print_vector(std::cout, in_data);

  std::vector<uint8_t> out_data = in_data;

  if (args.opr == Operation::encrypt) {
    std::reverse(out_data.begin(), out_data.end());
    if (false) {

      std::cerr << "Encryption Failed" << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    std::reverse(out_data.begin(), out_data.end());
    if (false) {
      std::cerr << "Decryption Failed" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  std::copy(out_data.begin(), out_data.end(),
            std::ostreambuf_iterator<char>(*args.os));

  if (!*args.os) {
    std::cerr << "Failed to write to output stream" << std::endl;
    exit(EXIT_FAILURE);
  }
  static_cast<std::ofstream *>(args.os.get())->close();
  exit(EXIT_SUCCESS);
}
