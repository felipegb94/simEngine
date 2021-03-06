cmake_minimum_required (VERSION 2.8)

project (simEngine)
# Using Armadillo
find_package( Armadillo REQUIRED )

find_library(MUPARSER_LIBRARY
             NAMES muparser
             DOC "The muParser binary" REQUIRED)

find_path(MUPARSER_INCLUDE_DIR
          NAMES muParser.h
          PATH_SUFFIXES muparser
          DOC "The muParser include directory")

include_directories(
${ARMADILLO_INCLUDE_DIRS} 
${BOOST_INCLUDE_DIRS}
${MUPARSER_INCLUDE_DIR} 
${CMAKE_SOURCE_DIR}/includes/
${CMAKE_SOURCE_DIR}/includes/rapidjson/
${CMAKE_SOURCE_DIR}/includes/symbolic_cpp/
${CMAKE_SOURCE_DIR}/source/
${CMAKE_SOURCE_DIR}/source/constraints/
${CMAKE_SOURCE_DIR}/source/forces/
${CMAKE_SOURCE_DIR}/source/json/
)
SET (SOURCE_FILES 
source/json/parseJSON.cpp
source/json/jsonFunctions.cpp
source/Body.cpp
source/Function.cpp
source/Model.cpp
source/Solver.cpp
source/myAverageStdDev.cpp
source/constraints/c_constraint.cpp
source/constraints/c_absX.cpp 
source/constraints/c_absY.cpp 
source/constraints/c_absAngle.cpp 
source/constraints/c_absDist.cpp 
source/constraints/c_relX.cpp 
source/constraints/c_relY.cpp 
source/constraints/c_relDist.cpp 
source/constraints/c_revJoint.cpp 
source/constraints/c_transJoint.cpp
source/forces/f_force.cpp
source/forces/f_pointForce.cpp
source/forces/f_torque.cpp
source/forces/f_rsda.cpp
source/forces/f_forceFile.cpp
source/forces/f_torqueFile.cpp


)
SET (INCLUDE_FILES
source/json/jsonParser.h
source/json/jsonFunctions.h
source/Body.h
source/constraints/c_constraint.h
source/Function.h
source/Model.h
source/Solver.h
source/averageStdDev.h
source/forces/f_force.h
)



#add_executable(gutierrez_cpp_03 assignments/assignment3.cpp  ${SOURCE_FILES})
#target_link_libraries(gutierrez_cpp_03 ${ARMADILLO_LIBRARIES}  ${MUPARSER_LIBRARY} )



add_executable(gutierrez_cpp_finalExam assignments/finalExam.cpp  ${SOURCE_FILES})
target_link_libraries(gutierrez_cpp_finalExam ${ARMADILLO_LIBRARIES} ${MUPARSER_LIBRARY} )




