cmake_minimum_required(VERSION 3.16)

project(real-desert-library VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(AVFORMAT_LIBRARY avformat)
find_library(AVCODEC_LIBRARY avcodec)
find_library(AVUTIL_LIBRARY avutil)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Widgets Network Qml Quick DBus Sql)
qt_standard_project_setup()

qt_add_executable(real-desert-library
    src/main.cpp
    src/app/application.cpp
    src/db/database.cpp
    src/db/files.cpp
    src/db/episodes.cpp
    src/db/shows.cpp
    src/db/cast-crew.cpp
    src/db/migration/migrate.cpp
    src/lib/add-show.cpp
    src/lib/media-meta.cpp
    src/lib/local-path.cpp
    src/model/show-search-result.cpp
    src/model/movie-search-result.cpp
    src/model/file.cpp
    src/model/episode.cpp
    src/model/episode-list-item.cpp
    src/model/person.cpp
    src/model/show.cpp
    src/model/show-filter.cpp
    src/model/show-list-item.cpp
    src/net//tmdb/show.cpp
    src/net//tmdb/movie.cpp
    src/ui/ui.cpp
    src/ui/file-handler.cpp
    src/ui/mode-handler.cpp
    src/ui/busy-handler.cpp
    src/ui/sort-order-wrapper.cpp
    src/ui/people/people-search.cpp
    src/ui/movies/movies.cpp
    src/ui/movies/movie-search-list.cpp
    src/ui/movie/movie.cpp
    src/ui/shows/shows.cpp
    src/ui/shows/show-search-list.cpp
    src/ui/shows/shows-genre-list.cpp
    src/ui/shows/shows-tags-list.cpp
    src/ui/shows/shows-list.cpp
    src/ui/shows/shows-filter.cpp
    src/ui/show/show.cpp
    src/ui/show/episode-list.cpp
    src/ui/show/episode-match.cpp
    src/ui/show/episode-details.cpp
)

qt_add_qml_module(real-desert-library
    URI com.realdesert.ui
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/pages/Show.qml
        qml/pages/Shows.qml
        qml/pages/Movies.qml
        qml/pages/About.qml
        qml/dialogs/EpisodeDetailDialog.qml
        qml/dialogs/ShowSearchDialog.qml
        qml/dialogs/MovieSearchDialog.qml
        qml/dialogs/EpisodeMatcherDialog.qml
        qml/dialogs/PeopleSearchDialog.qml
        qml/molecules/CheckField.qml
        qml/molecules/DefinitionList.qml
        qml/molecules/LongText.qml
        qml/molecules/Foldable.qml
        qml/molecules/SelectList.qml
        qml/molecules/SelectOne.qml
        qml/molecules/SortOrderCombo.qml
        qml/components/EpisodeFilter.qml
        qml/components/EpisodeList.qml
        qml/components/ShowsFilter.qml
        qml/components/ShowsList.qml
    RESOURCES
        images/desert-logo.svg
        images/config.svg
        images/acc-closed.svg
        images/acc-open.svg
        images/select-true.svg
        images/select-false.svg
        images/favorite.svg
        images/details.svg
        images/time.svg
        images/subtitles.svg
        images/sort-alpha-asc.svg
        images/sort-alpha-desc.svg
        images/sort-numeric-asc.svg
        images/sort-numeric-desc.svg
        images/sort-default.svg
)

target_link_libraries(real-desert-library
    PRIVATE
    Qt6::Core
    Qt6::Concurrent
    Qt6::Widgets
    Qt6::Network
    Qt6::Qml
    Qt6::Quick
    Qt6::DBus
    Qt6::Sql

    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
)

include(InstallRequiredSystemLibraries)

# Install rules
install(TARGETS real-desert-library
    DESTINATION bin
)

include(GNUInstallDirs)

install(FILES resources/real-desert-library.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

install(FILES images/desert-logo.svg
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
    RENAME real-desert-library.svg
)

# Package metadata
set(CPACK_PACKAGE_NAME "real-desert-library")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Tanya Randstoft <tanya@randstoft.dk>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tanya Randstoft")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "qml6-module-qtqml, qml6-module-qtqml-workerscript, qml6-module-qtquick, qml6-module-qtquick-controls,
     libqt6core6t64 (>= 6.4), libqt6gui6t64 (>= 6.4), libqt6widgets6t64 (>= 6.4),
     libqt6network6t64 (>= 6.4), libqt6qml6 (>= 6.4), libqt6quick6 (>= 6.4), libqt6dbus6 (>= 6.4), qml6-module-qtquick-templates (>= 6.4), qml6-module-qtquick-window (>= 6.4), libqt6svg6 (>= 6.4), libavformat60 (>= 6.0), libavcodec62 (>= 6.0), libavutil58 (>= 6.0)"
)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_SYSTEM_NAME "amd64")
set(CPACK_GENERATOR "DEB")

include(CPack)
