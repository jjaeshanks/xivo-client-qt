/* Copyright (C) 2015 Avencall
 *
 * XiVO Client is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with a Section 7 Additional
 * Permission as follows:
 *   This notice constitutes a grant of such permission as is necessary
 *   to combine or link this software, or a modified version of it, with
 *   the OpenSSL project's "OpenSSL" library, or a derivative work of it,
 *   and to copy, modify, and distribute the resulting work. This is an
 *   extension of the special permission given by Trolltech to link the
 *   Qt code with the OpenSSL library (see
 *   <http://doc.trolltech.com/4.4/gpl.html>). The OpenSSL library is
 *   licensed under a dual license: the OpenSSL License and the original
 *   SSLeay license.
 *
 * XiVO Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XiVO Client.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __PEOPLE_ACTIONS_H__
#define __PEOPLE_ACTIONS_H__

#include <QAction>
#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>

#include "people_entry.h"
#include "people_entry_model.h"

class PeopleActions: public QObject
{
    Q_OBJECT

    typedef QPair<QString, QString> QStringPair;

    public:
        PeopleActions();
        PeopleActions(const PeopleActions &other);
        QAction *newChatAction(QWidget *parent) const;
        QList<QAction *> getMailtoActions(QWidget *parent) const;
        bool hasChat() const;
        void setChatParams(const QString &name, const QString &xivo_uuid, int user_id);
        void setEmail(const QString &title, const QString &email);

    public slots:
        void chat();
        void mailto();

    private:
        QList<QStringPair> m_mailto_destinations;
        QVariantList m_chat_destination;
};

Q_DECLARE_METATYPE(PeopleActions);

#endif
