set(BIN_NAME dde-login-reminder)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network DBus REQUIRED)

find_package(PkgConfig REQUIRED)

message(${PROJECT_SOURCE_DIR})

add_executable(${BIN_NAME}
    main.cpp
    loginreminderinfo.h
    loginreminderinfo.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/utils.h
)

target_link_libraries(${BIN_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Network
)

target_include_directories(${BIN_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
)

install(TARGETS ${BIN_NAME} DESTINATION bin)

set(SYSTEMD_FILES
    dde-login-reminder.service
)
install(FILES ${SYSTEMD_FILES} DESTINATION lib/systemd/user/)
install(DIRECTORY DESTINATION lib/systemd/user/dde-osd.target.wants/)
install_symlink(dde-login-reminder.service dde-osd.target.wants)
