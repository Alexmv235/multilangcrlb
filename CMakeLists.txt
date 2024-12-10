cmake_minimum_required(VERSION 3.28)
project(rayintalex)

find_package(raylib REQUIRED)
find_package(cJSON REQUIRED)

set(CMAKE_C_STANDARD 23)

# Copiar archivos de idioma al directorio de salida
file(COPY i18n/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/i18n)
# Copiar archivos de idioma al directorio de salida
file(COPY resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

# Incluir la biblioteca fileLoaderSnap
# Suponiendo que el archivo .a está en la carpeta "libs" y el header está en "include"
if(UNIX AND NOT APPLE) # Asegurarse de que es Linux
    set(FILE_LOADER_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libs/libfileLoaderSnap.a)  # Biblioteca estática
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)  # Ruta al encabezado file_loader_snap.h
endif()

# Crear el ejecutable
add_executable(rayintalex main.c)

# Vincular las bibliotecas
target_link_libraries(rayintalex raylib cjson::cjson)
if(UNIX AND NOT APPLE) # Vincular solo en Linux
    target_link_libraries(rayintalex ${FILE_LOADER_LIB})  # Vincular la biblioteca estática
endif()

# Especificar los archivos que se instalarán
install(TARGETS rayintalex
        RUNTIME DESTINATION bin      # Instalación del binario
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/i18n
        DESTINATION bin    # Carpeta para los archivos de idioma
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources
        DESTINATION bin    # Carpeta para los archivos de idioma
)

# Instalar la biblioteca fileLoaderSnap si es Linux
if(UNIX AND NOT APPLE)
    install(FILES ${FILE_LOADER_LIB}
            DESTINATION lib    # Carpeta de instalación para bibliotecas
    )
endif()
