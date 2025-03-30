cmake_minimum_required(VERSION 3.10)
project(Voiture VERSION 1.0 LANGUAGES CXX)

# Configuration C++ moderne
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Trouver SQLite3
find_package(SQLite3 REQUIRED)

# Trouver wxWidgets (remplace GTK)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Configuration des sources
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Création de l'exécutable
add_executable(Voiture ${SOURCES})

# Configuration des includes
target_include_directories(Voiture PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

# Lier les bibliothèques
target_link_libraries(Voiture PRIVATE 
    ${SQLite3_LIBRARIES}
    ${wxWidgets_LIBRARIES}
)

# Options spécifiques à wxWidgets
if(WIN32)
    # Sous Windows, ajouter ces options
    target_compile_definitions(Voiture PRIVATE 
        __WXMSW__
        _UNICODE
        UNICODE
    )
elseif(APPLE)
    # Configuration spécifique macOS
    target_compile_definitions(Voiture PRIVATE 
        __WXOSX_COCOA__
    )
else()
    # Configuration Linux/Unix
    target_compile_definitions(Voiture PRIVATE 
        __WXGTK__
    )
endif()

# Installation optionnelle
install(TARGETS Voiture DESTINATION bin)