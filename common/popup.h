/* XiVO Client
 * Copyright (C) 2007-2009, Proformatique
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

/* $Revision$
 * $Date$
 */

#ifndef __POPUP_H__
#define __POPUP_H__

#include <QHash>
#include <QIODevice>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QVariant>
#include <QWidget>
#include "xmlhandler.h"
#ifdef USE_OUTLOOK
#include "outlook_contact.h"
#endif

class QLabel;
class QLineEdit;
class QPushButton;
class QUrl;
class QXmlInputSource;
class QXmlSimpleReader;

class UserInfo;
class RemarkArea;

/*! \brief Profile popup widget
 *
 * Constructed from the message received */
class Popup: public QWidget
{
    Q_OBJECT
public:
    //! Construct from a QIODevice used to read XML input
    Popup(const bool &,
          const UserInfo *,
          QWidget * parent = 0);
    void feed(QIODevice *, const bool &);
    void addInfoInternal(const QString &, const QString &);
    //! Add a Text field (name, value)
    void addInfoText(int, const QString &, const QString &);
    //! Add a url field
    void addInfoLink(int, const QString &, const QString &);
    void addInfoLinkX(int, const QString &, const QString &, const QString &);
    void addInfoLinkAuto(const QString &, const QString &);
    //! Add a Picture
    void addInfoPicture(int, const QString &, const QString &);
    //! Add a Phone number
    void addInfoPhone(int, const QString &, const QString &);
    //! Add a Phone number
    void addInfoPhoneURL(int, const QString &, const QString &);
    //! getter for the message
    void setMessage(const QString &, const QString &);
    //! access to the message
    const QHash<QString, QString> & message() const;
    //! getter for the message
    void setMessageTitle(const QString &);
    //! access to the message
    const QString & messagetitle() const;
    //! finalize the Construction of the window and show it
    void finishAndShow();
    void setSheetPopup(const bool &);
    bool sheetpopup();
    const QString & callAstid() const;
    const QString & callUniqueid() const;
    const QString & callContext() const;
    const QString & callChannel() const;
    const QString & callKind() const;
    bool systraypopup();
    bool focus();
    void setTitle(const QString &);
        
    void addAnyInfo(const QString &,
                    const QString &,
                    const QString &,
                    const QString &,
                    const QString &);
    void addDefForm(const QString &, const QString &);
    void update(QList<QStringList> &);
    QList<QStringList> & sheetlines();
    void addRemarkArea();
    void activateRemarkArea();
    void desactivateRemarkArea();
    void addRemark(const QVariantMap & entry);
    const QString & id() const { return m_id; };
    void setId(const QString & id) { m_id = id; };
signals:
    void wantsToBeShown(Popup *);        //!< sent when the widget want to show itself
    void actionCall(const QString &,
                    const QString &,
                    const QString &);        //!< sent when the widget wants to dial
    void actionFromPopup(const QString &, const QVariant &);
    void save(const QString &);
    void newRemarkSubmitted(const QString &, const QString &);
public slots:
    void streamNewData();                //!< new input data is available
    void streamAboutToClose();        //!< catch aboutToClose() signal from the socket
    void socketDisconnected();        //!< connected to disconnected() signal
    void socketError(QAbstractSocket::SocketError err);        //!< socket error handling
    void dialThisNumber();
    void dispurl(const QUrl &);
    void httpGetNoreply();
    void actionFromForm();
    void newRemark(const QString &);
protected:
    void closeEvent(QCloseEvent *);        //!< catch close event
private:
    void addInfoForm(int, const QString &);
    void saveandclose();
    void setEnablesOnForms();
    
    QIODevice * m_inputstream;        //!< input stream where the XML is read from
    /* the following properties are for XML parsing */
    //! QXmlInputSource constructed from m_inputstream
    QXmlInputSource * m_xmlInputSource;
    //! XML parser object.
    QXmlSimpleReader m_reader;
    const UserInfo * m_ui;
    //! Handler for event generated by the XML parser
    XmlHandler * m_handler;
    bool m_parsingStarted;                //! Is the XML already started or not ?
    //! layout for the widget : vertical box
    QVBoxLayout * m_vlayout;
    QHash<QString, QString> m_message;        //! Message property
    QString m_messagetitle;        //! Message title
    QLabel * m_title;        //! Sheet Title
    
    QString m_astid;
    QString m_context;
    QString m_uniqueid;
    QString m_channel;
    QString m_kind;
    
    bool m_sheetpopup;
    bool m_systraypopup;
    bool m_focus;
    bool m_urlautoallow;
    bool m_toupdate;
    bool m_sheetui;
    QWidget * m_sheetui_widget;
    QStringList m_orders;
    QList<QStringList> m_sheetlines;
    QHash<QString, QPushButton *> m_form_buttons;
    QHash<QString, QString> m_remoteforms;
    QVariantMap m_timestamps;
#ifdef USE_OUTLOOK
    COLContact m_OLContact;
    bool        m_bOLFound;
#endif
    RemarkArea * m_remarkarea;  //!< user editable area
    QString m_id;
};

#endif
