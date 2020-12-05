#--------------------------------------------------------------------------
# PathTracing project
#--------------------------------------------------------------------------

PROJECT(PathTracing)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("example" FILES ${files_example})

# INCLUDE (CTest)

ADD_EXECUTABLE(PathTracing ${files_example})
TARGET_LINK_LIBRARIES(PathTracing core render)
ADD_DEPENDENCIES(PathTracing core render)
