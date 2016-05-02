/* XiVO Client
 * Copyright (C) 2012-2016 Avencall
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

#ifndef __SWITCHBOARD_H__
#define __SWITCHBOARD_H__

#include <QObject>

#include <ipbxlistener.h>
#include <xletlib/functests.h>
#include <xletlib/xlet.h>

#include "ui_switchboard.h"

class QLabel;
class QueueEntriesModel;
class QueueEntriesSortFilterProxyModel;
class UserInfo;
class CurrentCall;

class Switchboard : public XLet, public IPBXListener
{
    Q_OBJECT
    FUNCTESTED

    public:
        Switchboard(QWidget *parent=0);
        ~Switchboard();
    public slots:
        void updateIncomingHeader(const QString &id, const QVariantList &entries);
        void updateWaitingHeader(const QString &id, const QVariantList &entries);
        void incomingCallClicked(const QModelIndex &index);
        void waitingCallClicked(const QModelIndex &index);
        void keyPressEvent(QKeyEvent *event);
        void queueEntryUpdate(const QString &queue_id, const QVariantList &entry);
        void updatePhoneStatus(const QString &queue_id);
        void postInitializationSetup();
        void focusOnIncomingCalls();
        void focusOnWaitingCalls();
    signals:
        void dialSuccess();
    private slots:
        void answerIncomingCall() const;
    private:
        void answerIncomingCall(const QString &unique_id) const;
        void retrieveCallOnHold(const QString & call_unique_id) const;
        void handleEnterKeys();
        bool hasIncomingCalls();
        void parseCommand(const QVariantMap &message);
        void parseCurrentCalls(const QVariantMap &message);
        void watch_switchboard_queue();
        void connectPhoneStatus() const;
        void setupUi();
        void subscribeCurrentCalls() const;
        void updatePhoneId();
        QString updatePhoneHintStatus();
        void onPhoneStatusChange();
        bool isSwitchboardQueue(const QString &queue_id) const;
        bool isSwitchboardHoldQueue(const QString &queue_id) const;
        bool isSearching() const;

        Ui::SwitchboardPanel ui;
        CurrentCall *m_current_call;

        QueueEntriesModel *m_incoming_call_model;
        QueueEntriesSortFilterProxyModel *m_incoming_call_proxy_model;

        QueueEntriesModel *m_waiting_call_model;
        QueueEntriesSortFilterProxyModel *m_waiting_call_proxy_model;

        QString m_phone_id;
        QString m_phone_hintstatus;
};

#endif
