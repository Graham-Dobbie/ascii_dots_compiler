#! /bin/sh

cd build ; g++ -O0 -g ../main.cpp ../parsetree/parsetree.cpp ../parsetree/segmenter.cpp ../parsetree/interpreter.cpp ../parsetree/flowanalyser.cpp ../parsetree/compiler.cpp -I ../parsetree/

