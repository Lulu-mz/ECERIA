cmake_minimum_required(VERSION 3.29)
project(Projet_ft__Supdepub C)

set(CMAKE_C_STANDARD 23)

add_executable(Projet_ft__Supdepub main.c
        Joueur.c
        Joueur.h
        Jeu.c
        Jeu.h
        Carte.c
        Carte.h
        Constantes.h
        Biome.c
        Biome.h
        Inventaire.c
        Inventaire.h)
IF(WIN32)
    target_link_libraries(Projet_ft__Supdepub -lmingw32 -lallegro-5.0.10-monolith-md-debug)
ELSE()
    INCLUDE_DIRECTORIES(/usr/local/Cellar/allegro/5.2.8.0/include)
    LINK_DIRECTORIES(/usr/local/Cellar/allegro/5.2.8.0/lib)
    file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.8.0/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")
    TARGET_LINK_LIBRARIES(Projet_ft__Supdepub ${LIBRARIES})
ENDIF(WIN32)
