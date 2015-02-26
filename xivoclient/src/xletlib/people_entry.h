/* XiVO Client
 * Copyright (C) 2014-2015 Avencall
 *
 * This file is part of XiVO Client.
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

#ifndef _PEOPLE_ENTRY_H_
#define _PEOPLE_ENTRY_H_

#include <QVariant>

#include <xletlib/xletlib_export.h>

class XLETLIB_EXPORT PeopleEntry
{
    public:
        PeopleEntry(const QVariantList &data, const QVariantMap &relations);
        PeopleEntry(const PeopleEntry &other);
        ~PeopleEntry();
        const QVariant data(int column) const;
        PeopleEntry &operator=(const PeopleEntry &other);
        const QString &xivoUuid() const;
        int agentId() const;
        int endpointId() const;
        int userId() const;
        QPair<QString, int> uniqueAgentId() const;
        QPair<QString, int> uniqueEndpointId() const;
        QPair<QString, int> uniqueUserId() const;

    private:
        QVariantList m_data;
        QString m_xivo_uuid;
        int m_agent_id;
        int m_user_id;
        int m_endpoint_id;
};

#endif /* _PEOPLE_ENTRY_H_ */