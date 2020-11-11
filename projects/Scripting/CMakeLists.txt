#--------------------------------------------------------------------------
# Scripting project
#--------------------------------------------------------------------------

PROJECT(Scripting)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(Scripting ${files_example})
TARGET_LINK_LIBRARIES(Scripting core render)
ADD_DEPENDENCIES(Scripting core render)
