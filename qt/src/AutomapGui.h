/*
 * Copyright (c) 2026  Jason Self <j@jxself.org>
 *
 *  This file is free software: you may copy, redistribute and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see https://www.gnu.org/licenses/.
 */

#ifndef TRIZBORT_AUTOMAPGUI_H
#define TRIZBORT_AUTOMAPGUI_H

#include <functional>

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEventLoop>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "TranscriptAutomapper.h"

namespace trizbort {

// The GUI implementation of the automapper's decision callbacks: modal
// disambiguation and same-direction dialogs, and an F11 single-step gate run on
// a nested event loop so the window stays responsive while mapping.
class GuiAutomapController : public AutomapController {
public:
    explicit GuiAutomapController(QWidget *parent)
        : m_parent(parent)
    {
    }

    void setStepping(bool stepping) { m_stepping = stepping; }
    bool stepping() const { return m_stepping; }
    // id -> short label shown in the disambiguation list.
    void setLabelFn(std::function<QString(int)> fn) { m_labelFn = std::move(fn); }
    void setStatusFn(std::function<void(const QString &)> fn) { m_statusFn = std::move(fn); }

    // Called by the window while an automap is running.
    void proceed()
    {
        if (m_loop)
            m_loop->quit();
    }
    void runToCompletion()
    {
        m_stepping = false;
        if (m_loop)
            m_loop->quit();
    }
    void cancel()
    {
        m_cancel = true;
        if (m_loop)
            m_loop->quit();
    }
    bool cancelled() const { return m_cancel; }

    int disambiguateRoom(const QString &name, const QList<int> &candidates) override
    {
        QDialog dialog(m_parent);
        dialog.setWindowTitle(QObject::tr("Which Room?"));
        auto *layout = new QVBoxLayout(&dialog);
        auto *list = new QListWidget(&dialog);
        for (int id : candidates) {
            const QString label = m_labelFn ? m_labelFn(id) : QObject::tr("Room %1").arg(id);
            auto *item = new QListWidgetItem(label, list);
            item->setData(Qt::UserRole, id);
        }
        auto *newItem = new QListWidgetItem(QObject::tr("(a new room)"), list);
        newItem->setData(Qt::UserRole, -1);
        list->setCurrentRow(0);
        layout->addWidget(new QLabel(
            QObject::tr("\"%1\" matches several rooms. Which did you enter?").arg(name), &dialog));
        layout->addWidget(list);
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dialog);
        QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(list, &QListWidget::itemActivated, &dialog, &QDialog::accept);
        layout->addWidget(buttons);
        dialog.exec();
        QListWidgetItem *chosen = list->currentItem();
        return chosen ? chosen->data(Qt::UserRole).toInt() : (candidates.isEmpty() ? -1 : candidates.first());
    }

    SameDir sameDirection(int fromRoomId, int existingRoomId, const QString &dir) override
    {
        Q_UNUSED(fromRoomId);
        Q_UNUSED(existingRoomId);
        QMessageBox box(m_parent);
        box.setWindowTitle(QObject::tr("Room Already There"));
        box.setText(QObject::tr("A different room already lies %1. Keep which?").arg(dir.toUpper()));
        QPushButton *existing = box.addButton(QObject::tr("Keep Existing"), QMessageBox::AcceptRole);
        QPushButton *both = box.addButton(QObject::tr("Keep Both"), QMessageBox::AcceptRole);
        QPushButton *fresh = box.addButton(QObject::tr("Keep New"), QMessageBox::AcceptRole);
        box.setDefaultButton(both);
        box.exec();
        if (box.clickedButton() == existing)
            return SameDir::KeepExisting;
        if (box.clickedButton() == fresh)
            return SameDir::KeepNew;
        return SameDir::KeepBoth;
    }

    bool step() override
    {
        if (m_cancel)
            return false;
        if (!m_stepping)
            return true;
        QEventLoop loop;
        m_loop = &loop;
        if (m_statusFn)
            m_statusFn(QObject::tr("Automap paused — F11 steps, Ctrl+F5 runs to the end, Stop cancels."));
        loop.exec();
        m_loop = nullptr;
        return !m_cancel;
    }

    void status(const QString &message) override
    {
        if (m_statusFn)
            m_statusFn(message);
    }

private:
    QWidget *m_parent = nullptr;
    bool m_stepping = false;
    bool m_cancel = false;
    QEventLoop *m_loop = nullptr;
    std::function<QString(int)> m_labelFn;
    std::function<void(const QString &)> m_statusFn;
};

// The options dialog shown before automapping (the C# AutomapDialog): transcript
// file, the toggles, and the customizable command words.
class AutomapOptionsDialog : public QDialog {
public:
    explicit AutomapOptionsDialog(const AutomapSettings &settings, QWidget *parent = nullptr)
        : QDialog(parent)
        , m_settings(settings)
    {
        setWindowTitle(tr("Automap"));
        auto *form = new QFormLayout(this);

        auto *fileRow = new QWidget(this);
        auto *fileLayout = new QHBoxLayout(fileRow);
        fileLayout->setContentsMargins(0, 0, 0, 0);
        m_file = new QLineEdit(fileRow);
        auto *browse = new QPushButton(tr("Browse…"), fileRow);
        fileLayout->addWidget(m_file);
        fileLayout->addWidget(browse);
        connect(browse, &QPushButton::clicked, this, [this] {
            const QString p = QFileDialog::getOpenFileName(
                this, tr("Transcript"), QString(),
                tr("Transcripts (*.txt *.log *.scr);;All files (*)"));
            if (!p.isEmpty())
                m_file->setText(p);
        });
        form->addRow(tr("&Transcript:"), fileRow);

        m_twoWay = new QCheckBox(tr("Assume connections are two-way"), this);
        m_twoWay->setChecked(settings.assumeTwoWayConnections);
        form->addRow(QString(), m_twoWay);
        m_sameName = new QCheckBox(tr("Rooms with the same name are the same room"), this);
        m_sameName->setChecked(settings.assumeSameNameSameRoom);
        form->addRow(QString(), m_sameName);
        m_verbose = new QCheckBox(tr("Transcript is verbose; expect room descriptions"), this);
        m_verbose->setChecked(settings.verboseTranscript);
        form->addRow(QString(), m_verbose);
        m_guessExits = new QCheckBox(tr("Guess exits from room descriptions"), this);
        m_guessExits->setChecked(settings.guessExits);
        form->addRow(QString(), m_guessExits);
        m_startFromEnd =
            new QCheckBox(tr("Start from the end of the transcript (map only new moves)"), this);
        m_startFromEnd->setChecked(settings.continueTranscript);
        form->addRow(QString(), m_startFromEnd);
        m_step = new QCheckBox(tr("Single-step with F11"), this);
        form->addRow(QString(), m_step);

        m_objectCmd = new QLineEdit(settings.objectCommand, this);
        form->addRow(tr("&Add-object command:"), m_objectCmd);
        m_regionCmd = new QLineEdit(settings.regionCommand, this);
        form->addRow(tr("Add-&region command:"), m_regionCmd);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        form->addRow(buttons);
    }

    QString transcriptPath() const { return m_file->text().trimmed(); }
    bool stepping() const { return m_step->isChecked(); }
    AutomapSettings settings() const
    {
        AutomapSettings s = m_settings;
        s.assumeTwoWayConnections = m_twoWay->isChecked();
        s.assumeSameNameSameRoom = m_sameName->isChecked();
        s.verboseTranscript = m_verbose->isChecked();
        s.continueTranscript = m_startFromEnd->isChecked();
        s.guessExits = m_guessExits->isChecked();
        s.objectCommand = m_objectCmd->text().trimmed().isEmpty() ? QStringLiteral("tb see")
                                                                  : m_objectCmd->text().trimmed();
        s.regionCommand = m_regionCmd->text().trimmed().isEmpty() ? QStringLiteral("tb region")
                                                                  : m_regionCmd->text().trimmed();
        return s;
    }

private:
    AutomapSettings m_settings;
    QLineEdit *m_file = nullptr;
    QCheckBox *m_twoWay = nullptr;
    QCheckBox *m_sameName = nullptr;
    QCheckBox *m_verbose = nullptr;
    QCheckBox *m_startFromEnd = nullptr;
    QCheckBox *m_guessExits = nullptr;
    QCheckBox *m_step = nullptr;
    QLineEdit *m_objectCmd = nullptr;
    QLineEdit *m_regionCmd = nullptr;
};

} // namespace trizbort

#endif // TRIZBORT_AUTOMAPGUI_H
