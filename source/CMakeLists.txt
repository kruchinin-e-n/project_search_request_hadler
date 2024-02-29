cmake_minimum_required(
	VERSION 
	3.11
)

project(
	project_search_server 
	CXX
)

set( 
	CMAKE_CXX_STANDARD 
	17
)

set(
	SOURCE_DIR 
	src
)

set(
	FILES_MAIN 
	"${SOURCE_DIR}/main.cpp"
)

set(
	FILES_DOCUMENT 
	"${SOURCE_DIR}/document.h" 
	"${SOURCE_DIR}/document.cpp"
)

set(
	FILES_CONCURRENT_MAP 
	"${SOURCE_DIR}/concurrent_map.h"
)

set(
	FILES_PAGINATOR 
	"${SOURCE_DIR}/paginator.h"
)

set(
	FILES_PROCESS_QUERIES 
	"${SOURCE_DIR}/process_queries.h" 
	"${SOURCE_DIR}/process_queries.cpp"
)

set(
	FILES_READ_INPUT_FUNCTIONS 
	"${SOURCE_DIR}/read_input_functions.h" 
	"${SOURCE_DIR}/read_input_functions.cpp"
)

set(
	FILES_REQUEST_QUEUE 
	"${SOURCE_DIR}/request_queue.h" 
	"${SOURCE_DIR}/request_queue.cpp")

set(
	FILES_SEARCH_SERVER 
	"${SOURCE_DIR}/search_server.h" 
	"${SOURCE_DIR}/search_server.cpp"
)

set(
	FILES_STRING_PROCESSING 
	"${SOURCE_DIR}/string_processing.h" 
	"${SOURCE_DIR}/string_processing.cpp")

set(
	FILES_TEST_EXAMPLE_FUNCTIONS 
	"${SOURCE_DIR}/test_example_functions.h" 
	"${SOURCE_DIR}/test_example_functions.cpp"
)

add_executable(
	search_server
	${FILES_MAIN}
	${FILES_DOCUMENT}
	${FILES_CONCURRENT_MAP}
	${FILES_PAGINATOR}
	${FILES_PROCESS_QUERIES}
	${FILES_READ_INPUT_FUNCTIONS}
	${FILES_REQUEST_QUEUE}
	${FILES_SEARCH_SERVER}
	${FILES_STRING_PROCESSING}
	${FILES_TEST_EXAMPLE_FUNCTIONS}
)

source_group(
	"Main"
	FILES ${FILES_MAIN}
)
source_group(
	"Document"
	FILES ${FILES_DOCUMENT}
)
source_group(
	"Concurrent map"
	FILES ${FILES_CONCURRENT_MAP}
)
source_group(
	"Paginator"
	FILES ${FILES_PAGINATOR}
)
source_group(
	"Process queries"
	FILES ${FILES_PROCESS_QUERIES}
)
source_group(
	"Read input functions"
	FILES ${FILES_READ_INPUT_FUNCTIONS}
)
source_group(
	"Request queue" 
	FILES ${FILES_REQUEST_QUEUE}
)
source_group(
	"Search server"
	FILES ${FILES_SEARCH_SERVER}
)
source_group(
	"String processing"
	FILES ${FILES_STRING_PROCESSING}
)
source_group(
	"Test example functions"
	FILES ${FILES_TEST_EXAMPLE_FUNCTIONS}
)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
	set(
		SYSTEM_LIBS 
		-lstdc++
	)
else()
	set(
		SYSTEM_LIBS
	)
endif()


target_link_libraries(
	search_server 
	${SYSTEM_LIBS}
)