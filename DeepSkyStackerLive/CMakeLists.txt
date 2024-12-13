set(PROJECT_NAME DeepSkyStackerLive)
message("Configuring project: " ${PROJECT_NAME})

set(COMPILE_WARNING_AS_ERROR YES)
SET (CMAKE_INSTALL_BINDIR ".")

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "dssliveenums.h"
    "LiveSettings.h"
 )
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__SmtpMime
    "../include/SmtpMime/emailaddress.h"
    "../include/SmtpMime/mimeattachment.h"
    "../include/SmtpMime/mimebase64encoder.h"
    "../include/SmtpMime/mimebase64formatter.h"
    "../include/SmtpMime/mimebytearrayattachment.h"
    "../include/SmtpMime/mimecontentencoder.h"
    "../include/SmtpMime/mimefile.h"
    "../include/SmtpMime/mimehtml.h"
    "../include/SmtpMime/mimeinlinefile.h"
    "../include/SmtpMime/mimemessage.h"
    "../include/SmtpMime/mimemultipart.h"
    "../include/SmtpMime/mimepart.h"
    "../include/SmtpMime/mimeqpencoder.h"
    "../include/SmtpMime/mimeqpformatter.h"
    "../include/SmtpMime/mimetext.h"
    "../include/SmtpMime/quotedprintable.h"
    "../include/SmtpMime/SmtpMime"
    "../include/SmtpMime/smtpmime_global.h"
)
source_group("Header Files\\SmtpMime" FILES ${Header_Files__SmtpMime})

set(Source_Files
    "ChartTab.cpp"
    "DeepSkyStackerLive.cpp"
    "EmailSettings.cpp"
    "fileregistrar.cpp"
    "filestacker.cpp"
    "foldermonitor.cpp"
    "footprint.cpp"
    "imageviewer.cpp"
    "LiveSettings.cpp"
    "progresslive.cpp"
    "RestartMonitoring.cpp"
    "settingstab.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Tools
    "../Tools/imageview.cpp"
    "../Tools/QLinearGradientCtrl.cpp"
)
source_group("Tools" FILES ${Tools})

set(i18n_Files
	"i18n/DeepSkyStackerLive_ca.ts"
	"i18n/DeepSkyStackerLive_cs.ts"
	"i18n/DeepSkyStackerLive_de.ts"
	"i18n/DeepSkyStackerLive_en.ts"
	"i18n/DeepSkyStackerLive_es.ts"
	"i18n/DeepSkyStackerLive_fr.ts"
	"i18n/DeepSkyStackerLive_it.ts"
	"i18n/DeepSkyStackerLive_ja_JP.ts"
	"i18n/DeepSkyStackerLive_nl.ts"
	"i18n/DeepSkyStackerLive_pt_BR.ts"
	"i18n/DeepSkyStackerLive_ro.ts"
	"i18n/DeepSkyStackerLive_ru.ts"
	"i18n/DeepSkyStackerLive_tr.ts"
	"i18n/DeepSkyStackerLive_zh_CN.ts"
	"i18n/DeepSkyStackerLive_zh_TW.ts"
	)	
source_group("Qt\\i18n" FILES ${i18n_Files} "i18n/translations.qrc"
    )
	
set(Resources
	"../DeepSkyStacker/Icon.rc"
	"../DeepSkyStacker/Icon/app.ico"
	"resources.qrc"
	"version.rc"
	)
source_group("Resources" FILES ${Resources}	)

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__SmtpMime}
    ${Source_Files}
	${i18n_Files} "i18n/translations.qrc"
	${Resources}
    ${Tools}
)

################################################################################
# Target
################################################################################
if ( ${QT_VERSION} VERSION_LESS 6.0.0)
add_executable(${PROJECT_NAME} ${ALL_FILES})
else()
qt_add_executable(${PROJECT_NAME} ${ALL_FILES})
endif()

set_source_files_properties(${i18n_Files}
     PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/i18n")

if ( ${QT_VERSION} VERSION_LESS 6.7.0)
if ( ${QT_VERSION} VERSION_LESS 6.2.0)
if ( ${QT_VERSION} VERSION_LESS 6.0.0)
  qt5_add_translation ( qmFiles ${i18n_Files})
else()
  message(FATAL_ERROR "qt_add_lrelease missing")
endif()
else()
  qt_add_lrelease(DeepSkyStackerLive TS_FILES ${i18n_Files})
endif()
else()
  qt_add_lrelease(TS_FILES ${i18n_Files})
endif()

set_target_properties(DeepSkyStackerLive PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
set_target_properties(DeepSkyStackerLive PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
)

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ../DeepSkyStackerKernel
    ../LibRaw
    ../ZClass
    ../Tools
  )

if(WIN32)
target_include_directories(${PROJECT_NAME} PUBLIC
   ../Zlib
   ../LibTiff
   ../CFitsIO
   ../include
   ../include/exiv2
   ../include/SmtpMime
   "$<$<CONFIG:Debug>:C:/Program Files (x86)/Visual Leak Detector/include>"
   "$ENV{Boost_1_80_0}"
)
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE
	stdafx.h
	)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
  "$<$<CONFIG:Debug>:Z_TRACE_DEVELOP>"
  "$<$<CONFIG:Release>:NDEBUG>"
  NOMINMAX
  LIBRAW_NODLL
  _CRT_SECURE_NO_DEPRECATE
  USE_LIBTIFF_STATIC
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Gm-;
            /Zi
        >
        $<$<CONFIG:Release>:
            /Zi;
            /GS-
        >
	    -W4 /wd4652 /wd4702 /wd4828 
		-Zc:__cplusplus /openmp:experimental
        /MP;
        /GL-;
        /EHsc
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /PROFILE
        >
        $<$<CONFIG:Release>:
            /OPT:ICF
        >
        /DEBUG:FULL;
        /MACHINE:X64;
        /SUBSYSTEM:WINDOWS;
        /DYNAMICBASE:NO
    )
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fopenmp
    )
endif()

if(WIN32)
target_link_directories(${PROJECT_NAME} PRIVATE
	"$<$<CONFIG:Debug>:C:/Users/amonra/Documents/GitHub/DSS/libs/Win64/DebugLibs>"
	"$<$<CONFIG:Release>:C:/Users/amonra/Documents/GitHub/DSS/libs/Win64/ReleaseLibs>"
	"$<$<CONFIG:Debug>:C:/Program Files (x86)/Visual Leak Detector/lib/Win64>"
	)
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Network
	Qt::Charts
	DeepSkyStackerKernel
    ZClass
)

if(WIN32)
set(DEBUG_DEPENDENCIES libraw libtiff exiv2d libexpatd zlibstaticd cfitsio)
set(RELEASE_DEPENDENCIES libraw libtiff exiv2 libexpat zlibstatic cfitsio)
else()
#set(DEBUG_DEPENDENCIES tiff.a exiv2.a expat.a z.a cfitsio.a brotlidec jpeg inih INIReader gomp)
# set(RELEASE_DEPENDENCIES tiff.a exiv2.a expat.a z.a cfitsio.a brotlidec jpeg inih INIReader gomp)
set(DEBUG_DEPENDENCIES raw-r.a tiff.a cfitsio.a brotlidec inih INIReader)
set(RELEASE_DEPENDENCIES raw-r.a tiff.a cfitsio.a brotlidec inih INIReader)
endif()

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:${DEBUG_DEPENDENCIES}>"
    "$<$<CONFIG:Release>:${RELEASE_DEPENDENCIES}>"
    "cfitsio"
    SmtpMime
)

if(WIN32)
set(ADDITIONAL_LIBRARY_DEPENDENCIES ${ADDITIONAL_LIBRARY_DEPENDENCIES}
    user32
    psapi
    htmlhelp
)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if(NOT LINUX)
    set (deploy_tool_options_arg "--libdir .")
    if(APPLE)
        set(deploy_tool_options_arg "${deploy_tool_options_arg} --hardened-runtime")
    elseif(WIN32)
        set(deploy_tool_options_arg "${deploy_tool_options_arg} --pdb")
    endif()
    # Generate a deployment script to be executed at install time
    # App bundles on macOS have an .app suffix
    if(APPLE)
        set(executable_path "$<TARGET_FILE_NAME:DeepSkyStackerLive>.app")
    else()
        message ("Qt Deploy Bin Dir: " ${QT_DEPLOY_BIN_DIR})
        message ("Qt Deploy Lib Dir: " ${QT_DEPLOY_LIB_DIR})
        message ("Target filename:"  $<TARGET_FILE_NAME:DeepSkyStackerLive>)
        set(executable_path "${CMAKE_INSTALL_BINDIR}/$<TARGET_FILE_NAME:DeepSkyStackerLive>")
    endif()
    message ("executable_path: " ${executable_path})
    message ("deploy tools options arg: " ${deploy_tool_options_arg})
     qt_generate_deploy_script(
         TARGET DeepSkyStackerLive
         OUTPUT_SCRIPT deploy_script
         CONTENT "
     qt_deploy_runtime_dependencies(
         EXECUTABLE \"${executable_path}\"
         DEPLOY_TOOL_OPTIONS ${deploy_tool_options_arg}
     )"
     )

INSTALL (TARGETS DeepSkyStackerLive)
INSTALL (SCRIPT ${deploy_script})
endif()
