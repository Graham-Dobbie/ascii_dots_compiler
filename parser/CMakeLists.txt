set(GEOMETRY_SRCS
   grammer.hpp grammer.cpp
   parser.hpp parser.cpp
   tokenizer.hpp tokenizer.cpp
)

SET (LIB_TYPE STATIC)

add_library (geometry ${LIB_TYPE} ${GEOMETRY_SRCS})