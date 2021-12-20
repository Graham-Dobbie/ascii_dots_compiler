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

    Segment(std::string type, std::vector<Cord> inputs, std::vector<Cord> outputs, std::vector<char> text_data);
    Segment(std::string type, std::vector<char> text_data);

    void print();

    std::string type;
    std::vector<Cord> inputs;
    std::vector<Cord> outputs;
    std::vector<char> text_data;

    bool isEnd();
};

// The Tokenizer class scans source code and separates it into tokens
class Segmenter {

  private:
    Cord _cursor = Cord();
    std::vector<std::vector<char>> text;

    Segment _followPath(Cord *c);

  public:

    Cord _step(Cord last, Cord current);
    Cord* _findChar();
    std::vector<Segment> segments;

    Segmenter();

    Segmenter(std::vector<std::string> raw_text);
    void setString(std::string m); // Gives sources code to tokenizer

    std::vector<Segment> getSegments(); // Returns the all segments of the source code

    bool isEnd(); // Returns true if the source code is finished
};

}

#endif