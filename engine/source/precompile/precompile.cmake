set(PRECOMPILE_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(VIRTUAL_PRECOMPILE_PARAMS_IN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source/precompile/precompile.json.in")
set(VIRTUAL_PRECOMPILE_PARAMS_PATH "${PRECOMPILE_TOOLS_PATH}/precompile.json")
configure_file(${VIRTUAL_PRECOMPILE_PARAMS_IN_PATH} ${VIRTUAL_PRECOMPILE_PARAMS_PATH})

# message(STATUS "messs: ${HEADER_FILES}")

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(PRECOMPILE_PRE_EXE)
    # set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/Linux/meta_parser)
    set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/VirtualEngineParser)
    # set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/PiccoloParser)
    set(sys_include "/usr/include/c++/11/")
    # execute_process(COMMAND chmod a+x ${PRECOMPILE_PARSER} WORKING_DIRECTORY ${PRECOMPILE_TOOLS_PATH})
endif()

set(PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h)

# ###BUILDING
set(PRECOMPILE_TARGET "VirtualEnginePreCompile")

add_custom_target(${PRECOMPILE_TARGET} ALL
COMMAND ${CMAKE_COMMAND} -E echo "**************************************** "
COMMAND ${CMAKE_COMMAND} -E echo "**** [Precompile] BEGIN ****"
COMMAND ${CMAKE_COMMAND} -E echo "**************************************** "

COMMAND ${PRECOMPILE_PARSER} "${VIRTUAL_PRECOMPILE_PARAMS_PATH}" "${PARSER_INPUT}" "${ENGINE_ROOT_DIR}/source" ${sys_include} "VE" 0

COMMAND ${CMAKE_COMMAND} -E echo "++++ Precompile finished ++++"
)