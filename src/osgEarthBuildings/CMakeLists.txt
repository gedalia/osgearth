
IF(OSGEARTH_BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DOSGEARTHBUILDINGS_LIBRARY)
ELSE ()
    ADD_DEFINITIONS(-DOSGEARTHBUILDINGS_LIBRARY_STATIC)
ENDIF()

set(LIB_NAME osgEarthBuildings)

set(HEADER_PATH ${OSGEARTH_SOURCE_DIR}/include/${LIB_NAME})

set(LIB_PUBLIC_HEADERS
    Analyzer
    BuildContext
    Building
    BuildingCatalog
    BuildingCompiler
    BuildingFactory
    BuildingLayer
    BuildingLayerOptions
    BuildingPager
    BuildingSymbol
    BuildingVisitor
    Common
    Compiler
    CompilerOutput
    CompilerSettings
    InstancedBuildingCompiler
    InstancedRoofCompiler
    Elevation
    ElevationCompiler
    Export
    FlatRoofCompiler
    GableRoofCompiler
    Parapet
    Roof
    Zoning
	ExternalModelNode
	InstancedModelNode
	ElevationsLodNode
	FilterUsage
)

set(LIB_COMMON_FILES
    Analyzer.cpp
    Building.cpp
    BuildingCatalog.cpp
    BuildingCompiler.cpp
    BuildingFactory.cpp
    BuildingLayer.cpp
    BuildingPager.cpp
    BuildingSymbol.cpp
    BuildingVisitor.cpp
    Compiler.cpp
    CompilerOutput.cpp
    CompilerSettings.cpp
    InstancedBuildingCompiler.cpp
    InstancedRoofCompiler.cpp
    Elevation.cpp
    ElevationCompiler.cpp
    FeaturePlugin.cpp
    FlatRoofCompiler.cpp
    GableRoofCompiler.cpp
    Parapet.cpp
    Roof.cpp
    ExternalModelNode.cpp
    InstancedModelNode.cpp
    ElevationsLodNode.cpp
)

ADD_LIBRARY( ${LIB_NAME} SHARED
    ${LIB_PUBLIC_HEADERS}  
    ${LIB_COMMON_FILES}
)

# Add this project to the NodeKits solution folder
set_property( TARGET ${LIB_NAME} PROPERTY FOLDER "NodeKits" )


INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR} ${OSGEARTH_SOURCE_DIR} ${GDAL_INCLUDE_DIR})

IF (TRACY_FOUND)
    INCLUDE_DIRECTORIES(${TRACY_INCLUDE_DIR})
    LINK_WITH_VARIABLES(${LIB_NAME} TRACY_LIBRARY)
ENDIF(TRACY_FOUND)

IF (WIN32)
  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
ELSE(WIN32)
  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
ENDIF(WIN32)

LINK_INTERNAL(${LIB_NAME}
    osgEarth
)

LINK_WITH_VARIABLES(${LIB_NAME} OSG_LIBRARY OSGUTIL_LIBRARY OSGDB_LIBRARY OSGTEXT_LIBRARY OSGGA_LIBRARY OPENTHREADS_LIBRARY GDAL_LIBRARY)
LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)
