add_library (parsetree parsetree.cpp)
add_library (segmenter segmenter.cpp)


target_link_libraries(segmenter PRIVATE parsetree)