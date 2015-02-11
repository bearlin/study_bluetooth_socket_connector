############################################################################################################
# Function for a better printfing system per component
#
# Key:
# GENERAL
############################################################################################################
function(print QUALCOSA)
	if(GENERATION_MESSAGES)
		message(STATUS "[${PROJECT_NAME}] ${QUALCOSA}")
	endif(GENERATION_MESSAGES)
endfunction(print QUALCOSA)

############################################################################################################
# This function applies the three basic properties for let the Visual Studio not complaining on 
# the CMake Generated projects.
#
# Key:
# GENERAL
# PERFORCE
############################################################################################################
macro(add_msvc_perforce_properties)
	if(${PERFORCE_ENABLED})
		SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY VS_SCC_PROJECTNAME "Perforce Project")
		SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY VS_SCC_LOCALPATH "${BRANCH_ROOT}")
		SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY VS_SCC_PROVIDER "MSSCCI:Perforce SCM")
	endif(${PERFORCE_ENABLED})
endmacro(add_msvc_perforce_properties)

############################################################################################################
# This function applies the three basic properties for let the Visual Studio not complaining on 
# the CMake Generated projects.
#
# Key:
# GENERAL
# PERFORCE
############################################################################################################
macro(add_msvc_perforce_properties_name NAME_OF_THE_TARGET)
	if(${PERFORCE_ENABLED})
		SET_PROPERTY(TARGET ${NAME_OF_THE_TARGET} PROPERTY VS_SCC_PROJECTNAME "Perforce Project")
		SET_PROPERTY(TARGET ${NAME_OF_THE_TARGET} PROPERTY VS_SCC_LOCALPATH "${BRANCH_ROOT}")
		SET_PROPERTY(TARGET ${NAME_OF_THE_TARGET} PROPERTY VS_SCC_PROVIDER "MSSCCI:Perforce SCM")
	endif(${PERFORCE_ENABLED})
endmacro(add_msvc_perforce_properties_name  NAME_OF_THE_TARGET)

############################################################################################################
# Function that given a list of files and a path it returns a list of the files found recursively from
# that path
#
# Key:
# GENERAL
# PROJECT_CONFIGURATION
############################################################################################################
macro(recursive_find_files VAR_LIST_OF_FILE_NAMES VAR_STARTING_PATH VAR_OUTPUT_LIST)
	foreach (FILE ${VAR_LIST_OF_FILE_NAMES})
		file(GLOB_RECURSE FILE_PATH_TEMP ${VAR_STARTING_PATH}/${FILE})
  		list(APPEND ${VAR_OUTPUT_LIST} ${FILE_PATH_TEMP})
	endforeach()
endmacro()

############################################################################################################
# Scans the current directory and returns a list of subdirectories excluding from 
# the return list the directories given in the 4th parameter
# 1st parameter - return value
# 2nd parameter - directory where looking for directory
# 3rd parameter - relativization
# 4th parameter - Parameters to exclude in the search
#
# Key:
# GENERAL
############################################################################################################
macro(filtered_list_subdirectories retval curdir relative_to dir_to_exclude)
  string(REPLACE " " ";" n_dir_to_exclude ${${dir_to_exclude}})
  file(GLOB subdir RELATIVE_PATH "${curdir}/*")
	SET(list_of_dirs "")
	FOREACH(dir2 ${subdir})
		file(RELATIVE_PATH dir ${relative_to} ${dir2})
    list(FIND n_dir_to_exclude ${dir} dir2_found)
		IF(dir2_found EQUAL -1)
			IF(IS_DIRECTORY ${dir2})
				SET(list_of_dirs ${list_of_dirs} ${dir})
			ENDIF(IS_DIRECTORY ${dir2})
		ENDIF(dir2_found EQUAL -1)
	ENDFOREACH(dir2 ${subdir})
  SET(${retval} ${list_of_dirs})
ENDMACRO(filtered_list_subdirectories retval curdir relative_to dir_to_exclude)

################################################################################################
# It include external archive library to the linker 
# 1st parameter - library to add as input
# 2nd parameter - path of the location of the .a
# For a target Something.a a new Something.a_external target to link will be created
#
# Key:
# EXPERIMENTAL
################################################################################################
FUNCTION(add_external_unix_archive ARCHIVE_NAME ARCHIVE_PATH)
  IF(ARCHIVE_NAME)
    IF(ARCHIVE_PATH)
      add_library(${ARCHIVE_NAME}_external STATIC IMPORTED)
      set_property(TARGET ${ARCHIVE_NAME}_external PROPERTY IMPORTED_LOCATION ${ARCHIVE_PATH}/${ARCHIVE_NAME})
    ENDIF(ARCHIVE_PATH)
  ENDIF(ARCHIVE_NAME)
ENDFUNCTION(add_external_unix_archive ARCHIVE_NAME ARCHIVE_PATH)

################################################################################################
# It wraps an external library in a cmake target that can be linked by other regular 
# cmake targets. For windows the lib symbol file has to be used anyway
# 1st parameter - CMake target name that will wrap the externa library
# 2nd parameter - library (full) path
#
# Key:
# EXPERIMENTAL
################################################################################################
function(add_external_library EXTERNAL_TARGET_NAME EXTERNAL_TARGET_FULL_PATH)
  add_library(${EXTERNAL_TARGET_NAME}_lib STATIC IMPORTED)
  set_property(TARGET ${EXTERNAL_TARGET_NAME}_lib PROPERTY IMPORTED_LOCATION ${EXTERNAL_TARGET_FULL_PATH} GLOBAL)
endfunction(add_external_library EXTERNAL_TARGET_NAME EXTERNAL_TARGET_FULL_PATH)

############################################################################################
# Adds a compiler flags to all source files given in the list SOURCE_FILES
############################################################################################
function(add_compiler_flags_for_source_files SOURCE_FILES ADDED_COMPILER_FLAGS)

  # SOURCE_FILES is a variable containing a variable of the list of source files
  # ${SOURCE_FILES} is a variable containing the list of source files
  # ${${SOURCE_FILES}} is a list of source files

  foreach(source_file ${${SOURCE_FILES}}) 
    set_source_files_properties("${source_file}" PROPERTIES COMPILE_FLAGS ${ADDED_COMPILER_FLAGS})
  endforeach(source_file ${SOURCE_FILES})
endfunction(add_compiler_flags_for_source_files SOURCE_FILES ADDED_COMPILER_FLAGS)

############################################################################################################
# This function strips out the '-Werror" flag from the current C and C++ compiler flags.
# This should only be used for external projects which contain warnings we are not in control of.
# Remember to re-enable them after your particular module. Note this relies on specific ordering of flags
# in the default setup, so be careful when changing them.
#
# Key:
# GENERAL
############################################################################################################
macro(disable_warnings_as_errors_on_target PROJECT_NAME)
  if(UNIX)
    print("Disabling warning as error in ${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wno-error")
  endif(UNIX)
endmacro(disable_warnings_as_errors_on_target PROJECT_NAME)

############################################################################################################
# Macro to enable function tracing on a specific target
# (This function is only for Android)
############################################################################################################
macro(function_trace_target TARGET_NAME)
  if(ANDROID)
    SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "-fno-function-sections -pg -fno-omit-frame-pointer")
  endif(ANDROID)
endmacro(function_trace_target)

###############################################################################
# Extract the base path of PROJECT_BINARY_DIR
# Name: get_base_path
# Parameter:
#   VAR_OUTPUT: Variable will contain extracted base path
###############################################################################
function(get_base_path VAR_OUTPUT)
    string(REGEX MATCH "${GENERATION_ROOT}/(.*)" RESULT ${PROJECT_BINARY_DIR})
    if(${CMAKE_MATCH_1} STREQUAL "")
        message(FATAL_ERROR "Could not extract base path for ${PROJECT_NAME}")
    endif(${CMAKE_MATCH_1} STREQUAL "")
    set(${VAR_OUTPUT} ${CMAKE_MATCH_1} PARENT_SCOPE)
endfunction(get_base_path VAR_OUTPUT)

###############################################################################
# Get output directory for current project
# Name get_current_output_directory
# Parameter:
#   VAR_OUTPUT_DIR: Variable will contain the current output directory
###############################################################################
function(get_current_output_directory VAR_OUTPUT_DIR)
    get_base_path(BASE_PATH)
    set(${VAR_OUTPUT_DIR} ${OUTPUT_ROOT}/${BASE_PATH} PARENT_SCOPE)
endfunction(get_current_output_directory VAR_OUTPUT_DIR)

###############################################################################
# Get source directory for current project
# Name: get_current_source_directory
# Parameter:
#   VAR_SOURCE_DIR: Variable will contain the current source directory
###############################################################################
function(get_current_source_directory VAR_SOURCE_DIR)
    get_base_path(BASE_PATH)
    set(${VAR_SOURCE_DIR} ${SOURCE_ROOT}/${BASE_PATH} PARENT_SCOPE)
endfunction(get_current_source_directory VAR_SOURCE_DIR)

###############################################################################
# Set destination for archive, library and runtime output for current project
# Name: set_build_output_directories
# Parameter:
#   VAR_PROJECT_NAME: Name of project
#   VAR_OUTPUT_DIR: Location of binary output
###############################################################################
function(set_target_output_directory VAR_PROJECT_NAME VAR_OUTPUT_DIR)
    set_property(TARGET ${VAR_PROJECT_NAME} PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${VAR_OUTPUT_DIR})
    set_property(TARGET ${VAR_PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${VAR_OUTPUT_DIR})
    set_property(TARGET ${VAR_PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${VAR_OUTPUT_DIR})
endfunction(set_target_output_directory VAR_PROJECT_NAME VAR_OUTPUT_DIR)

###############################################################################
# Set the solution folder for MSVC IDE. Ignored by other generator
# Parameter:
#   VAR_PROJECT_NAME: Name of project
#   VAR_REL_OUTPUT_DIR: Relative Location of binary output/VS IDE folders
###############################################################################
function(set_target_msvc_solution_folder VAR_PROJECT_NAME VAR_REL_OUTPUT_DIR)
  set_target_properties(${VAR_PROJECT_NAME} PROPERTIES FOLDER ${VAR_REL_OUTPUT_DIR})
endfunction(set_target_msvc_solution_folder VAR_PROJECT_NAME VAR_REL_OUTPUT_DIR)

###############################################################################
# Set the solution folder for MSVC IDE from the project source directory.
# Ignored by other generator
###############################################################################
function(set_target_msvc_solution_folder_from_project_directory)
  # convert "." to "/"
  STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" PROJECT_FOLDER ${PROJECT_SOURCE_DIR})
  set_target_properties(${VAR_PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_FOLDER})
endfunction()

###############################################################################
# Name: create_library
# Parameter:
#  VAR_LIB_NAME: Name of library
#  VAR_SRCS: source files that should be included in library
#  ARGV2: library dependencies (optional)
#  ARGV3: STATIC | SHARED (optional)
###############################################################################
function(create_library VAR_LIB_NAME VAR_SRCS)
    set(VAR_DEPENDENCIES ${${ARGV2}})

    # NOTE: This is a dependency to global variable ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY:
    #       if you want to have added compiler flags for all source files of the library,
    #       you need to set this variable.
    if(NOT WIN32 AND NOT ${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY} STREQUAL "")
        message(STATUS "Setting additional compiler flags '${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY}' for ${VAR_LIB_NAME}")
        add_compiler_flags_for_source_files(${VAR_SRCS} ${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY})
    endif()
    
    add_library(${VAR_LIB_NAME} ${ARGV3} ${${VAR_SRCS}})
    
    # Special linking to android std libraries and starting symbol for the library
    if(ANDROID)
     if (${ARGV3} AND ${ARGV3} STREQUAL "SHARED")
        list(APPEND VAR_DEPENDENCIES ${ANDROID_STD_TOOLCHAIN_SHAREDLIB})
     else()
        list(APPEND VAR_DEPENDENCIES ${ANDROID_STD_TOOLCHAIN_STATICLIB})
     endif()
    endif(ANDROID)

    if(VAR_DEPENDENCIES)
        target_link_libraries(${VAR_LIB_NAME} ${VAR_DEPENDENCIES})
    endif()

    if(WIN32)
        add_msvc_perforce_properties_name(${VAR_LIB_NAME})
        set_target_msvc_solution_folder_from_project_directory()
    endif(WIN32)
endfunction(create_library VAR_LIB_NAME VAR_SRCS)

###############################################################################
# Name: create_executable
# Parameter:
#  VAR_EXE_NAME: Name of executable
#  VAR_SRCS: source files that should be included in the executable
#  ARGV2: library dependencies (optional)
#  ARGV3: EXCLUDE_FROM_ALL (optional)
###############################################################################
function(create_executable VAR_EXE_NAME VAR_SRCS)
    set(VAR_DEPENDENCIES ${${ARGV2}})

    # NOTE: This is a dependency to global variable ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY:
    #       if you want to have added compiler flags for all source files of the library,
    #       you need to set this variable.
    if(NOT WIN32 AND NOT ${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY} STREQUAL "")
        message(STATUS "Setting additional compiler flags '${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY}' for ${VAR_EXE_NAME}")
        add_compiler_flags_for_source_files(${VAR_SRCS} ${ADDED_COMPILER_FLAGS_FOR_CREATE_LIBRARY})
    endif()

    add_executable(${VAR_EXE_NAME} ${ARGV3} ${${VAR_SRCS}})

    # Special linking to android std libraries and starting symbol for the executable
    if(ANDROID)
      list(APPEND VAR_DEPENDENCIES ${ANDROID_STD_TOOLCHAIN_EXEC})
    endif(ANDROID)

    # Libs needed for x86 Application
    if(UNIX AND NOT ANDROID)
      list(APPEND VAR_DEPENDENCIES
        pthread
        dl
        rt
      )
    endif(UNIX AND NOT ANDROID)

    if(VAR_DEPENDENCIES)
      target_link_libraries(${VAR_EXE_NAME} ${VAR_DEPENDENCIES})
    endif()

    if(WIN32)
      add_msvc_perforce_properties_name(${VAR_EXE_NAME})
      set_target_msvc_solution_folder_from_project_directory()
    endif(WIN32)
endfunction(create_executable VAR_EXE_NAME VAR_SRCS)

###############################################################################
# Name: append a string to another string passed as variable
# Parameter:
#  VAR: Variable of the first string
#  STRING_TO_APPEND: string to be appended
###############################################################################
macro(append_string VAR STRING_TO_APPEND)
  set(${VAR} "${${VAR}}${STRING_TO_APPEND}")
endmacro(append_string VAR STRING_TO_APPEND)


###############################################################################
# Name: Wrapping function that has same functionalities of file(GLOB ...
#       but ensure that the list is returned in alphabetical order
#       Note that passed arguments are not explicitly listed; this function
#       uses ARGN to pass variable number of arguments
# Parameter:
#  VAR: variable of the resultant list
#  ARGN: variadic number of patterns
###############################################################################
function(file_glob VAR)
  file(GLOB LOCAL_VAR ${ARGN})
  list(SORT LOCAL_VAR)
  set(${VAR} ${LOCAL_VAR} PARENT_SCOPE)
endfunction()

###############################################################################
# Name: Wrapping function that has same functionalities of file(GLOB_RECURSE
#       but ensure that the list is returned in alphabetical order
#       Note that passed arguments are not explicitly listed; this function
#       uses ARGN to pass variable number of arguments
# Parameter:
#  VAR: variable of the resultant list
#  ARGN: variadic number of patterns
###############################################################################
function(file_glob_recurse VAR)
  file(GLOB_RECURSE LOCAL_VAR ${ARGN})
  list(SORT LOCAL_VAR)
  set(${VAR} ${LOCAL_VAR} PARENT_SCOPE)
endfunction()

###############################################################################
# Name: 
# Parameter:
#  VAR: 
#  PATTERN: 
###############################################################################

function(check_min_ccache_size)
  if(UNIX AND NOT NO_CCACHE)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Build/System/Util/ccachesizetest.py 5 ${CMAKE_BINARY_DIR}/ccachemaxsize.bsdat)
  endif(UNIX AND NOT NO_CCACHE)
endfunction(check_min_ccache_size)
