/***********************************************************************
 *
 * Copyright (C) 2013 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef DAILY_PROGRESS_H
#define DAILY_PROGRESS_H

class Preferences;

#include <QAbstractTableModel>
#include <QDate>
#include <QStringList>
#if (QT_VERSION >= QT_VERSION_CHECK(4,7,0))
#include <QElapsedTimer>
#else
#include <QTime>
#endif
#include <QVector>
class QSettings;

class DailyProgress : public QAbstractTableModel
{
	Q_OBJECT

public:
	DailyProgress(QObject* parent = 0);
	~DailyProgress();

	void findCurrentStreak(QDate& start, QDate& end) const;
	void findLongestStreak(QDate& start, QDate& end) const;
	int percentComplete();

	void increaseWordCount(int words);
	void increaseTime();
	void loadPreferences(const Preferences& preferences);

	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	static void setPath(const QString& path);

public slots:
	void save();
	void setProgressEnabled(bool enable = true);

signals:
	void progressChanged();
	void streaksChanged();

private:
	void findStreak(int pos, int& start, int& end) const;
	void updateProgress();

private:
	QSettings* m_file;

	int m_words;
	int m_msecs;
	int m_type;
	int m_goal;

#if (QT_VERSION >= QT_VERSION_CHECK(4,7,0))
	QElapsedTimer m_typing_timer;
#else
	QTime m_typing_timer;
#endif

	class Progress
	{
	public:
		Progress(const QDate& date = QDate()) : m_date(date), m_progress(0)
			{ }

		QDate date() const
			{ return m_date; }

		int progress() const
			{ return m_progress; }

		void setDate(const QDate& date)
			{ m_date = date; }

		void setProgress(int words, int msecs, int type, int goal);

	private:
		QDate m_date;
		int m_progress;
	};
	QVector<Progress> m_progress;
	Progress* m_current;
	bool m_current_valid;
	int m_current_pos;
	int m_progress_enabled;

	QStringList m_day_names;

	static QString m_path;
};

inline void DailyProgress::increaseWordCount(int words)
{
	m_words += words;
	m_current_valid = false;
	updateProgress();
}

#endif
