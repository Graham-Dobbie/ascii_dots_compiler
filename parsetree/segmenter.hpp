#ifndef SEGMENTER_HPP_INCLUDED
#define SEGMENTER_HPP_INCLUDED

#include "parsetree.hpp"

namespace segmenter {

// Cord class is a 2d vector that points to a place on the parse field
class Cord {
  public:
    Cord();
    Cord(int x, int y);

    int x;
    int y;

    bool operator==(const Cord &v);
    Cord operator+(const Cord &v);
    Cord operator-(const Cord &v);
    Cord operator*(const int s);

    std::vector<Cord> getNeighbors();

    void print();
};

// Token class stores the type and value the tokenizer scans from the source code
class Segment {

  public:
    Segment();

    Segment(std::string type, std::vector<Cord> cords, std::vector<std::pair<Cord, Cord>> outlets, std::vector<char> text_data);
    Segment(std::string type, std::vector<char> text_data);

    void print();

    std::string type;
    std::vector<Cord> cords;
    std::vector<std::pair<Cord, Cord>> outlets;
    std::vector<char> text_data;

    bool isEnd();
};

// The Tokenizer class scans source code and separates it into tokens
class Segmenter {

  private:
    std::vector<std::vector<char>> text;

    std::vector<Segment> _segs;

    char _lib_warp;
    std::vector<std::string> _libs_used;
    std::vector<char> _warps;

    void _fuction_data_scan();
    void _deleteComments();

    Cord *_findDot();

    Cord _getDot(Cord c);
    void _getOp(Cord c);
    void _getMerger(Cord c);

    void _eraseSeg(std::vector<Cord> cords);

    std::pair<Cord, Cord> _step(Cord src, Cord dir);
    void _followPath(Cord start, Cord next, bool begining);

    Segment _getQuote(Segment seg, Cord src, Cord dir);
    Segment _followDirection(bool direction);

    std::vector<Cord> _validNieghbors(Cord c);

  public:
    Segmenter();
    Segmenter(std::vector<std::string> raw_text);

    std::vector<Segment> getSegments(); // Returns the all segments of the source code

    void printText(); // Prints text
    void cropText();  // Crops text

    bool isEnd(); // Returns true if the source code is finished
};

} // namespace

#endif