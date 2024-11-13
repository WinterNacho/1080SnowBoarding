    cmake_minimum_required(VERSION 3.20)

    project(1080SnowBoarding_sln CXX)

    # Setting up "reasonable/questionable" defaults for a simple Ogre project.
    # Most of the features are OFF as we want a very simple first project.
    option(OGRE_BUILD_COMPONENT_BITES "" ON)
    option(OGRE_BUILD_COMPONENT_BULLET "" OFF)
    option(OGRE_BUILD_COMPONENT_CSHARP "" OFF)
    option(OGRE_BUILD_COMPONENT_JAVA "" OFF)
    option(OGRE_BUILD_COMPONENT_MESHLODGENERATOR "" OFF)
    # required for bites, imgui and native dialog
    option(OGRE_BUILD_COMPONENT_OVERLAY "" ON) 
    option(OGRE_BUILD_COMPONENT_OVERLAY_IMGUI "" ON)
    option(OGRE_BUILD_COMPONENT_PAGING "" OFF)
    option(OGRE_BUILD_COMPONENT_PROPERTY "" OFF)
    option(OGRE_BUILD_COMPONENT_PYTHON "" OFF)
    option(OGRE_BUILD_COMPONENT_RTSHADERSYSTEM "" ON)
    option(OGRE_BUILD_COMPONENT_TERRAIN "" OFF)
    option(OGRE_BUILD_COMPONENT_VOLUME "" OFF)
    # it will build dependencies that we do not need and everything in Debug configuration.
    # Still, it makes our lifes easier. This building process happens at cmake configure
    # time. So the first configure will take some time.
    option(OGRE_BUILD_DEPENDENCIES "" ON) 
    option(OGRE_BUILD_PLUGIN_ASSIMP "" ON)
    option(OGRE_BUILD_PLUGIN_BSP "" OFF)
    option(OGRE_BUILD_PLUGIN_DOT_SCENE "" OFF)
    option(OGRE_BUILD_PLUGIN_GLSLANG "" OFF)
    option(OGRE_BUILD_PLUGIN_OCTREE "" OFF)
    option(OGRE_BUILD_PLUGIN_PCZ "" OFF)
    option(OGRE_BUILD_PLUGIN_PFX "" OFF)
    option(OGRE_BUILD_PLUGIN_RSIMAGE "" OFF)
    option(OGRE_BUILD_PLUGIN_STBI "" ON)
    option(OGRE_BUILD_RENDERSYSTEM_D3D11 "" OFF)
    option(OGRE_BUILD_RENDERSYSTEM_D3D9 "" OFF)
    option(OGRE_BUILD_RENDERSYSTEM_GL "" OFF)
    option(OGRE_BUILD_RENDERSYSTEM_GL3PLUS "" ON)
    option(OGRE_BUILD_RENDERSYSTEM_GLES2 "" OFF)
    option(OGRE_BUILD_RENDERSYSTEM_TINY "" OFF)
    option(OGRE_BUILD_RENDERSYSTEM_VULKAN "" OFF)
    option(OGRE_BUILD_SAMPLES "" OFF)
    option(OGRE_BUILD_TEST "" OFF)
    option(OGRE_BUILD_TOOLS "" OFF)
    option(OGRE_INSTALL_DEPENDENCIES "" ON)
    option(OGRE_INSTALL_DOCS "" OFF)
    option(OGRE_INSTALL_PDB "" OFF)
    option(OGRE_INSTALL_SAMPLES "" OFF)
    option(OGRE_INSTALL_TOOLS "" OFF)
    add_subdirectory("ThirdParty/ogre")
    add_subdirectory("ThirdParty/rapidcsv")
    # At the end of Ogre cmake configuration, you will get the following 
    # (or similar) summary printed:
    #[[
    Building components:
    + Bites
    + Overlay
    + Overlay Dear Imgui
    + RTShader System
    + RTShader System Shaders
    Building plugins:
    + STBI codec (generic)
    + Assimp loader (.obj, .ply, .fbx and more)
    Building rendersystems:
    + OpenGL 3+
    Building core features:
    + Mesh Lod
    + DDS image codec (.dds)
    + ETC image codec (.pkm, .ktx)
    + ASTC image codec (.astc)
    + ZIP archives
    + Multi-threaded WorkQueue

    Build type:                      shared
    Use double precision:            disabled
    Assert mode:                     exceptions
    #]]

    add_executable(1080SnowBoarding
        "src/main.cpp"
        "src/rider.cpp"
        "src/rider.h"
        "src/utils.cpp"
        "src/utils.h"
        "src/ramp.cpp"
        "src/ramp.h"
        "src/tree.cpp"
        "src/tree.h"
    )

    set_property(TARGET 1080SnowBoarding PROPERTY CXX_STANDARD 20)

    # we are using ogre headers from the install directory, so you need to build and install ogre
    # targets before building 1080SnowBoarding.
    target_include_directories(1080SnowBoarding PRIVATE 
        "${CMAKE_INSTALL_PREFIX}/include/OGRE"
        "${CMAKE_INSTALL_PREFIX}/include/OGRE/Bites"
        "${CMAKE_INSTALL_PREFIX}/include/OGRE/RTShaderSystem"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/rapidcsv/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/miniaudio"
    )
    target_link_libraries(1080SnowBoarding PRIVATE
        OgreBites
    )

    # installing all build files
    install(TARGETS 1080SnowBoarding
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT 1080SnowBoarding_Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT 1080SnowBoarding_Runtime
            NAMELINK_COMPONENT 1080SnowBoarding_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT 1080SnowBoarding_Development
    )

    # installing assets folder
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # When executing the exe from visual studio, we use the install directory as the working directory,
    # this is to find the asset folder and configuration files. We need to run the install target before
    # hand so those files are placed there.
    set_target_properties( 1080SnowBoarding
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
    )