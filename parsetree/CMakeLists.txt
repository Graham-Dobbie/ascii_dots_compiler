add_library (parsetree parsetree.cpp)
add_library (segmenter segmenter.cpp)
add_library (compiler compiler.cpp)
add_library (flowanalyser flowanalyser.cpp)
add_library (interpreter interpreter.cpp)

target_link_libraries(parsetree PRIVATE compiler)
target_link_libraries(segmenter PRIVATE compiler)
target_link_libraries(flowanalyser PRIVATE compiler)
target_link_libraries(interpreter PRIVATE compiler)

target_link_libraries(segmenter PRIVATE parsetree)