#pragma once

#include "migration.h"

namespace Rd {
    namespace Database {
        namespace Migration {
            class Optimize : public Migration {
            public:
                int version() const override { return 2; }
                QSqlError apply(const QSqlDatabase& db) override {
                    using CreateFn = QSqlError (Optimize::*)(const QSqlDatabase&);

                    std::vector<CreateFn> optimizationFunctions = {
                        &Optimize::finalize
                    };

                    for (auto fn : optimizationFunctions) {
                        QSqlError err = (this->*fn)(db);
                        if (err.type() != QSqlError::NoError) {
                            return err;
                        }
                    }

                    return QSqlError();
                }
            private:
                QSqlError optimizeFindEpisodes(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("\
  CREATE INDEX idx_episodes_show_id ON episodes(show_id);\
  CREATE INDEX idx_file_links_episode_id ON episode_files(episode_id);\
                    ");
                    query.exec();
                    return query.lastError();
                }

                QSqlError optimizeShowsByCast(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("\
  CREATE INDEX idx_cast_name ON cast_crew(name);\
  CREATE INDEX idx_cast_original_name ON cast_crew(original_name);\
  CREATE INDEX idx_show_cast_cast_id ON show_cast(cast_crew_id);\
  CREATE INDEX idx_episode_cast_cast_id ON episode_cast(cast_crew_id);\
                    ");
                    query.exec();
                    return query.lastError();
                }

                QSqlError finalize(const QSqlDatabase& db) {
                    QSqlQuery query(db);
                    query.prepare("ANALYZE");
                    query.exec();
                    return query.lastError();
                }
            };
        }
    }
}
