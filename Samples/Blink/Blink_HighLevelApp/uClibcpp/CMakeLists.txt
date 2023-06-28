#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

set(project_name uClibcpp)
project(${project_name} CXX)


set(rootdir ${CMAKE_SOURCE_DIR}/uClibcpp)
set(sources 
    ${rootdir}/algorithm.cpp
    ${rootdir}/associative_base.cpp
    ${rootdir}/bitset.cpp
    ${rootdir}/char_traits.cpp
    ${rootdir}/complex.cpp
    ${rootdir}/del_op.cpp
    ${rootdir}/del_opnt.cpp
    ${rootdir}/del_ops.cpp
    ${rootdir}/del_opv.cpp
    ${rootdir}/del_opvnt.cpp
    ${rootdir}/del_opvs.cpp
    ${rootdir}/deque.cpp
    ${rootdir}/eh_alloc.cpp
    ${rootdir}/eh_globals.cpp
    ${rootdir}/exception.cpp
    ${rootdir}/func_exception.cpp
    ${rootdir}/iomanip.cpp
    ${rootdir}/ios.cpp
    ${rootdir}/iostream.cpp
    ${rootdir}/istream.cpp
    ${rootdir}/iterator.cpp
    ${rootdir}/limits.cpp
    ${rootdir}/list.cpp
    ${rootdir}/locale.cpp
    ${rootdir}/map.cpp
    ${rootdir}/new_handler.cpp
    ${rootdir}/new_op.cpp
    ${rootdir}/new_opnt.cpp
    ${rootdir}/new_opv.cpp
    ${rootdir}/new_opvnt.cpp
    ${rootdir}/numeric.cpp
    ${rootdir}/ostream.cpp
    ${rootdir}/queue.cpp
    ${rootdir}/set.cpp
    ${rootdir}/sstream.cpp
    ${rootdir}/stack.cpp
    ${rootdir}/stdexcept.cpp
    ${rootdir}/streambuf.cpp
    ${rootdir}/string.cpp
    ${rootdir}/support.cpp
    ${rootdir}/typeinfo.cpp
    ${rootdir}/utility.cpp
    ${rootdir}/valarray.cpp
    ${rootdir}/vector.cpp
)


# include_directories(
# 	${rootdir}/include
# )

add_library(${project_name} STATIC ${sources})

