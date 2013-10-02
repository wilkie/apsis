##########################################################################
#  Copyright (c) 2009-2012 Organic Vectory B.V.
#  Written by George van Venrooij
#
#  Contributions by:   
#      Fredrik Orderud (https://sourceforge.net/users/fredrior/)
#      Ian Cullinan    (https://sourceforge.net/users/cibyr/)
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file license.txt)
##########################################################################

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED program_options)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

configure_file(version.hpp.cfg	${CMAKE_CURRENT_BINARY_DIR}/version.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable	(	clcc 
					main.cpp 
									version.hpp.cfg
					version.cpp		${CMAKE_CURRENT_BINARY_DIR}/version.hpp 
									output.hpp
					options.cpp		options.hpp 
					clew.c			clew.h 
					clpp.cpp		clpp.hpp 
					clinfo.cpp		clinfo.hpp 
									clexception.hpp
					test.cl 
					test_error.cl
					test_multifile.cl
				)

if (UNIX)
	target_link_libraries(clcc ${CMAKE_DL_LIBS})
endif()

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(clcc ${Boost_LIBRARIES})
endif()

source_group("Test Files" FILES test.cl test_error.cl test_multifile.cl)

add_test(no_arguments   	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc"                                            											)
add_test(help           	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --help                                     											)
add_test(version        	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --version                                  											)
add_test(info           	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --info                                     											)
add_test(test_cl        	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --add_headers "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"      								)
add_test(test_cl_err    	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" "${CMAKE_CURRENT_SOURCE_DIR}/test_error.cl"											)
add_test(test_cl_multi_err	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" "${CMAKE_CURRENT_SOURCE_DIR}/test_multifile.cl"										)
add_test(test_cl_multi		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --add_headers "${CMAKE_CURRENT_SOURCE_DIR}/test_multifile.cl" "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"	)
add_test(test_cl_out       	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --add_headers "${CMAKE_CURRENT_SOURCE_DIR}/test.cl" --output_file test.o      						)
add_test(test_cl_out_multi	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --add_headers "${CMAKE_CURRENT_SOURCE_DIR}/test_multifile.cl" "${CMAKE_CURRENT_SOURCE_DIR}/test.cl" -o test_multifile.o	)

add_test(cl_opt1         	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --device_type=CPU --cloptions "-cl-unsafe-math-optimizations -DMY_PI=3.1415" "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"	)
add_test(cl_opt2         	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --device_type=GPU --cloptions -DMY_PI=3.1415 "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"	)
add_test(dev_acc        	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --device_type=ACCELERATOR "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"	)
add_test(dev_cpu        	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --device_type=CPU "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"			)
add_test(dev_gpu        	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --device_type=GPU "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"			)
add_test(dev_cpu_err       	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --platform_filter CUDA --device_type=CPU "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"			)
add_test(dev_gpu_err       	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --platform_filter Intel --device_type=GPU "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"			)
add_test(index_0_0		    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --platform_index 0 --device_index 0 "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"			)
add_test(index_1_0			"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" -p 1 -d 0 "${CMAKE_CURRENT_SOURCE_DIR}/test.cl"									)

if (WIN32)
    add_test(clpath         	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --clpath "C:/Windows/System32/opencl.dll"  											)
endif()

if (APPLE)
    add_test(clpath         	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clcc" --clpath "/System/Library/Frameworks/OpenCL.Framework/OpenCL"  )
endif()

set_tests_properties	(	test_cl_err 
							test_cl_multi_err 
							dev_acc
							dev_cpu_err
							dev_gpu_err
							
							PROPERTIES	
								WILL_FAIL	TRUE
						)

#	Installation instructions
install	(	TARGETS				clcc
			RUNTIME	DESTINATION	bin
			COMPONENT			runtime
		)
