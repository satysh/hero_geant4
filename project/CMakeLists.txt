#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(HERO)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

# Setup of ROOT ANALYSIS : optional.
find_package(ROOT QUIET)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})

file(
  GLOB sources ${PROJECT_SOURCE_DIR}/src/HERODetectorConstruction.cc
               ${PROJECT_SOURCE_DIR}/src/HERODetector.cc
               ${PROJECT_SOURCE_DIR}/src/HEROActionInitialization.cc
               ${PROJECT_SOURCE_DIR}/src/HERORunAction.cc
               ${PROJECT_SOURCE_DIR}/src/HEROPrimaryGenerator.cc
               ${PROJECT_SOURCE_DIR}/src/HEROPhysicsList.cc
)
file(
  GLOB headers ${PROJECT_SOURCE_DIR}/include/HERODetectorConstruction.hh
               ${PROJECT_SOURCE_DIR}/include/HERODetector.hh
               ${PROJECT_SOURCE_DIR}/include/HEROActionInitialization.hh
               ${PROJECT_SOURCE_DIR}/include/HERORunAction.hh
               ${PROJECT_SOURCE_DIR}/include/HEROPrimaryGenerator.hh
               ${PROJECT_SOURCE_DIR}/include/HEROPhysicsList.hh
)
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(hero hero.cc ${sources} ${headers})
target_link_libraries(hero ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build HERO. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
#set(HERO_SCRIPTS
#  hero.in
#)

#foreach(_script ${HERO_SCRIPTS})
#  configure_file(
#    ${PROJECT_SOURCE_DIR}/${_script}
#    ${PROJECT_BINARY_DIR}/${_script}
#    COPYONLY
#    )
#endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(HERO DEPENDS hero)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS hero DESTINATION bin)

