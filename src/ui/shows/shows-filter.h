#pragma once

#include <QObject>
#include "model/show-filter.h"
#include "db/shows.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowsFilter : public QObject {
                Q_OBJECT
                Q_PROPERTY(quint16 newerThan READ newerThan WRITE setNewerThan NOTIFY filterUpdated)
                Q_PROPERTY(quint16 olderThan READ olderThan WRITE setOlderThan NOTIFY filterUpdated)
                Q_PROPERTY(bool watching READ watching WRITE setWatching NOTIFY filterUpdated)
                Q_PROPERTY(QVariantList genres READ genres NOTIFY filterUpdated)
                Q_PROPERTY(QVariantList tags READ tags NOTIFY filterUpdated)
            public:
                explicit ShowsFilter(QObject* parent = nullptr);
                ~ShowsFilter() noexcept;

                void init();
                void clear();

                quint16 newerThan();
                void setNewerThan(quint16 value);

                quint16 olderThan();
                void setOlderThan(quint16 value);

                bool watching();
                void setWatching(bool watching);

                QVariantList genres();
                Q_INVOKABLE void toggleGenre(quint16 genre);

                QVariantList tags();
                Q_INVOKABLE void toggleTag(quint16 tag);

                ShowFilter filter();
                Q_SIGNAL void filterUpdated();

                Q_INVOKABLE void setPerson(quint32 id);

                Q_INVOKABLE void reset();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Database::Shows* m_db;
                QVariantList m_genres;
                QVariantList m_tags;
                ShowFilter m_filter;
            };
        }
    }
}
