// Copyright (C) 2013 Christopher Miles
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_int32(seed, 0, "Pseudorandom number generator seed");
DEFINE_int32(board_size, 5, "Number of cells on the board in each dimension");
DEFINE_int32(cell_size, 100, "Width of the bingo board's cells (in pixels)");
DEFINE_string(words_list, "", "File containing words used to create the board");
DEFINE_string(gv_output, "", "File containing the Graphviz output");

using namespace std;
typedef google::int32 int32;

void remove_duplicates(vector<string>* words) {
  CHECK_NOTNULL(words);
  sort(words->begin(), words->end());
  vector<string>::iterator i = unique(words->begin(), words->end());
  words->resize(distance(words->begin(), i));
}

void write_to_file_or_die(const string& filename, const string& buffer) {
  ofstream writer(filename.c_str());
  CHECK(!writer.fail());
  writer << buffer;
  writer.close();
}

void check_inputs() {
  CHECK(FLAGS_cell_size >= 0);
  CHECK(FLAGS_board_size >= 0);
  CHECK(!FLAGS_words_list.empty());
  CHECK(!FLAGS_gv_output.empty());
}

void create_board(const int32 cell_size,
		  const int32 board_size,
		  const vector<string>& words,
		  string* board) {
  CHECK_NOTNULL(board);

  stringstream out;
  out << "digraph G {" << endl
      << "node [shape=plaintext]" << endl
      << "a1 [label=<" << endl << endl
      << "<TABLE border=\"5\" cellspacing=\"5\" cellpadding=\"10\" bgcolor=\"white\">" << endl;

  for (int32 row = 0; row < board_size; ++row) {
    out << "<TR>" << endl;
    for (int32 col = 0; col < board_size; ++col) {
      out << "<TD border=\"3\" fixedsize=\"true\" width=\"" << cell_size << "\" "
	  << "height=\"" << cell_size << "\">"
	  << words[row * board_size + col]
	  << "</TD>" << endl;
    }
    out << "</TR>" << endl;
  }

  out << "</TABLE>" << endl
      << ">];" << endl
      << "}";

  *board = out.str();
}

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  srand(FLAGS_seed);
  check_inputs();

  ifstream input_stream(FLAGS_words_list.c_str());
  CHECK(!input_stream.fail());

  // TODO(cmiles) handle multi-word lines
  // TODO(cmiles) split long lines into multiple rows
  istream_iterator<string> input_iter(input_stream);
  istream_iterator<string> eos;
  vector<string> words(input_iter, eos);
  remove_duplicates(&words);

  // Minimum number of words to populate the board without repeats
  int32 min_words = FLAGS_board_size * FLAGS_board_size;
  CHECK(words.size() >= min_words);

  // Board consists of the first min_words elements
  random_shuffle(words.begin(), words.end());

  string board;
  create_board(FLAGS_cell_size, FLAGS_board_size, words, &board);
  write_to_file_or_die(FLAGS_gv_output, board);
}
