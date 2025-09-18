#pragma once

#include "migration.h"

namespace Rd {
    namespace Database {
        namespace Migration {
            class Initial : public Migration {
            public:
                int version() const override { return 1; }
                QSqlError apply(const QSqlDatabase& db) override {
                    using CreateFn = QSqlError (Initial::*)(const QSqlDatabase&);

                    std::vector<CreateFn> creationFunctions = {
                        &Initial::createFilesTable,
                        &Initial::createGenresTable,
                        &Initial::createTagsTable,
                        &Initial::createCastCrewTable,
                        &Initial::createPlaybacksTable,
                        &Initial::createMoviesTable,
                        &Initial::createMovieFilesTable,
                        &Initial::createMovieGenresTable,
                        &Initial::createMovieTagsTable,
                        &Initial::createMovieCastTable,
                        &Initial::createShowsTable,
                        &Initial::createShowGenresTable,
                        &Initial::createShowTagsTable,
                        &Initial::createShowCastTable,
                        &Initial::createEpisodesTable,
                        &Initial::createEpisodeFilesTable,
                        &Initial::createEpisodeCastTable,
                        &Initial::createExtrasTable,
                        &Initial::createPlaylistsTable,
                        &Initial::createPlaylistFilesTable
                    };

                    for (auto fn : creationFunctions) {
                        QSqlError err = (this->*fn)(db);
                        if (err.type() != QSqlError::NoError) {
                            return err;
                        }
                    }

                    return QSqlError();
                }
            private:

                QSqlError createFilesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE files (\
  id INTEGER PRIMARY KEY,\
  path TEXT NOT NULL UNIQUE,\
  created TEXT,\
  selected_subtitle TEXT,\
  selected_video INTEGER,\
  selected_audio INTEGER,\
  meta TEXT,\
  runtime INTEGER,\
  subtitles TEXT,\
  video TEXT,\
  audio TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createGenresTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE genres (\
  id INTEGER PRIMARY KEY,\
  name TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createTagsTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE tags (\
  id INTEGER PRIMARY KEY,\
  name TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createCastCrewTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE cast_crew (\
  id INTEGER PRIMARY KEY,\
  name TEXT,\
  original_name TEXT,\
  profile_path TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createPlaybacksTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE playbacks (\
  file_id INTEGER NOT NULL,\
  time INTEGER,\
  playback INTEGER,\
  FOREIGN KEY (file_id) REFERENCES files(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createMovieFilesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE movie_files (\
  file_id INTEGER NOT NULL,\
  movie_id INTEGER NOT NULL,\
  FOREIGN KEY (file_id) REFERENCES files(id) ON DELETE CASCADE,\
  FOREIGN KEY (movie_id) REFERENCES movies(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createMoviesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE movies (\
  id INTEGER PRIMARY KEY,\
  imdb TEXT UNIQUE,\
  title TEXT,\
  original_title TEXT,\
  origin TEXT,\
  release_date TEXT,\
  tagline TEXT,\
  overview TEXT,\
  languages TEXT,\
  runtime INTEGER,\
  poster_path TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createMovieGenresTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE movie_genres (\
  movie_id INTEGER NOT NULL,\
  genre_id INTEGER NOT NULL,\
  FOREIGN KEY (movie_id) REFERENCES movies(id) ON DELETE CASCADE,\
  FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createMovieTagsTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE movie_tags (\
  movie_id INTEGER NOT NULL,\
  tag_id INTEGER NOT NULL,\
  FOREIGN KEY (movie_id) REFERENCES movies(id) ON DELETE CASCADE,\
  FOREIGN KEY (tag_id) REFERENCES tags(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createMovieCastTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE movie_cast (\
  movie_id INTEGER NOT NULL,\
  cast_crew_id INTEGER NOT NULL,\
  role TEXT,\
  job TEXT,\
  FOREIGN KEY (movie_id) REFERENCES movies(id) ON DELETE CASCADE,\
  FOREIGN KEY (cast_crew_id) REFERENCES cast_crew(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createEpisodeFilesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE episode_files (\
  file_id INTEGER NOT NULL,\
  episode_id INTEGER NOT NULL,\
  FOREIGN KEY (file_id) REFERENCES files(id) ON DELETE CASCADE,\
  FOREIGN KEY (episode_id) REFERENCES episodes(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createEpisodesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE episodes (\
  id INTEGER PRIMARY KEY,\
  show_id INTEGER NOT NULL,\
  season INTEGER,\
  episode INTEGER,\
  name TEXT,\
  overview TEXT,\
  air_date TEXT,\
  runtime INTEGER,\
  favorite INTEGER,\
  FOREIGN KEY (show_id) REFERENCES shows(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createEpisodeCastTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE episode_cast (\
  episode_id INTEGER NOT NULL,\
  cast_crew_id INTEGER NOT NULL,\
  role TEXT,\
  job TEXT,\
  FOREIGN KEY (episode_id) REFERENCES episodes(id) ON DELETE CASCADE,\
  FOREIGN KEY (cast_crew_id) REFERENCES cast_crew(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createShowCastTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE show_cast (\
  show_id INTEGER NOT NULL,\
  cast_crew_id INTEGER NOT NULL,\
  role TEXT,\
  job TEXT,\
  FOREIGN KEY (show_id) REFERENCES show(id) ON DELETE CASCADE,\
  FOREIGN KEY (cast_crew_id) REFERENCES cast_crew(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createShowsTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE shows (\
  id INTEGER PRIMARY KEY,\
  imdb TEXT UNIQUE,\
  name TEXT,\
  original_name TEXT,\
  origin TEXT,\
  tagline TEXT,\
  overview TEXT,\
  languages TEXT,\
  poster_path TEXT\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createShowGenresTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE show_genres (\
  show_id INTEGER NOT NULL,\
  genre_id INTEGER NOT NULL,\
  FOREIGN KEY (show_id) REFERENCES shows(id) ON DELETE CASCADE,\
  FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createShowTagsTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE show_tags (\
  show_id INTEGER NOT NULL,\
  tag_id INTEGER NOT NULL,\
  FOREIGN KEY (show_id) REFERENCES show(id) ON DELETE CASCADE,\
  FOREIGN KEY (tag_id) REFERENCES tags(id) ON DELETE CASCADE\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createExtrasTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE extras (\
  file_id INTEGER NOT NULL,\
  show_id INTEGER,\
  episode_id INTEGER,\
  movie_id INTEGER,\
  FOREIGN KEY (file_id) REFERENCES files(id) ON DELETE CASCADE,\
  FOREIGN KEY (show_id) REFERENCES shows(id) ON DELETE SET NULL,\
  FOREIGN KEY (episode_id) REFERENCES episodes(id) ON DELETE SET NULL,\
  FOREIGN KEY (movie_id) REFERENCES movies(id) ON DELETE SET NULL\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createPlaylistFilesTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE playlist_files (\
  file_id INTEGER NOT NULL,\
  playlist_id INTEGER NOT NULL,\
  sequence INTEGER,\
  FOREIGN KEY (file_id) REFERENCES files(id) ON DELETE CASCADE,\
  FOREIGN KEY (playlist_id) REFERENCES playlists(id) ON DELETE CASCADE,\
  UNIQUE (playlist_id, sequence)\
)");
                    query.exec();
                    return query.lastError();
                }

                QSqlError createPlaylistsTable(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("CREATE TABLE playlists (\
  id INTEGER PRIMARY KEY,\
  title TEXT UNIQUE NOT NULL\
)");
                    query.exec();
                    return query.lastError();
                }
            };
        }
    }
}
