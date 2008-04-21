/* XIVO CTI clients
 * Copyright (C) 2007, 2008  Proformatique
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking the Licensed Program statically or dynamically with other
 * modules is making a combined work based on the Licensed Program. Thus,
 * the terms and conditions of the GNU General Public License version 2
 * cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of the
 * Licensed Program give you permission to combine the Licensed Program
 * with free software programs or libraries that are released under the
 * GNU Library General Public License version 2.0 or GNU Lesser General
 * Public License version 2.1 or any later version of the GNU Lesser
 * General Public License, and with code included in the standard release
 * of OpenSSL under a version of the OpenSSL license (with original SSLeay
 * license) which is identical to the one that was published in year 2003,
 * or modified versions of such code, with unchanged license. You may copy
 * and distribute such a system following the terms of the GNU GPL
 * version 2 for the Licensed Program and the licenses of the other code
 * concerned, provided that you include the source code of that other code
 * when and as the GNU GPL version 2 requires distribution of source code.
*/

/* $Revision$
 * $Date$
 */

#include <QGridLayout>
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QApplication>
#include <QMenu>
#include <QDebug>

#include "baseengine.h"
#include "extendedlabel.h"
#include "peerchannel.h"
#include "peerwidget.h"
#include "xivoconsts.h"

/*! \brief Constructor
 */
PeerWidget::PeerWidget(const QString & id,
                       const QString & name,
                       const QHash<QString, QPixmap> & persons,
                       const QHash<QString, QPixmap> & phones,
                       const QHash<QString, QPixmap> & agents)
	: m_id(id), m_name(name), m_phones(phones), m_persons(persons), m_agents(agents)
{
	//qDebug() << "PeerWidget::PeerWidget()" << id;
	//	QHBoxLayout * layout = new QHBoxLayout(this);
        QFrame * qvline = new QFrame(this);
        qvline->setFrameShape(QFrame::VLine);
        qvline->setLineWidth(2);

	QGridLayout * layout = new QGridLayout(this);
	layout->setSpacing(2);
	layout->setMargin(2);
	m_statelbl = new QLabel();
	m_availlbl = new QLabel();
        m_agentlbl = new ExtendedLabel();
        m_voicelbl = new QLabel();
        m_fwdlbl   = new QLabel();

	m_statelbl->setPixmap( m_phones["grey"] );
	m_availlbl->setPixmap( m_persons["grey"] );
        m_agentlbl->setPixmap( m_agents["grey"] );
        //         m_voicelbl->setPixmap(  );
        //         m_fwdlbl->setPixmap(  );

	m_textlbl = new QLabel(m_name.isEmpty() ? tr("(No callerid yet)") : m_name,
                               this);
        if(m_name.isEmpty())
                qDebug() << "PeerWidget::PeerWidget()" << "the callerid information m_name is empty for :" << m_id;
	// set TextInteraction Flags so the mouse clicks are not catched by the
	// QLabel widget
	m_textlbl->setTextInteractionFlags( Qt::NoTextInteraction );
        layout->addWidget( qvline, 0, 0, 2, 1 );
	layout->addWidget( m_textlbl, 0, 2, 1, 6, Qt::AlignLeft );
	layout->addWidget( m_availlbl, 1, 2, Qt::AlignLeft );
	layout->addWidget( m_statelbl, 1, 3, Qt::AlignLeft );
	layout->addWidget( m_agentlbl, 1, 4, Qt::AlignLeft );
        //         layout->addWidget( m_voicelbl, 1, 5, Qt::AlignLeft );
        //         layout->addWidget( m_fwdlbl,   1, 6, Qt::AlignLeft );
	layout->setColumnStretch(8, 1);

        connect( m_agentlbl, SIGNAL(dial(QMouseEvent *)),
                 this, SLOT(mouseDoubleClickEventAgent(QMouseEvent *)) );

	// to be able to receive drop
	setAcceptDrops(true);
	m_removeAction = new QAction( tr("&Remove"), this);
	m_removeAction->setStatusTip( tr("Remove this peer from the panel") );
	connect( m_removeAction, SIGNAL(triggered()),
	         this, SLOT(removeFromPanel()) );
	m_dialAction = new QAction( tr("&Call"), this);
	m_dialAction->setStatusTip( tr("Call this peer") );
	connect( m_dialAction, SIGNAL(triggered()),
	         this, SLOT(dial()) );
}

/*! \brief destructor
 */
PeerWidget::~PeerWidget()
{
        //qDebug() << "PeerWidget::~PeerWidget()";
        clearChanList();
}

void PeerWidget::setRed(int n)
{
	//m_square.fill( Qt::red );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["red"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["red"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["red"]);
}

void PeerWidget::setGreen(int n)
{
	//m_square.fill( Qt::green );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["green"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["green"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["green"]);
}

void PeerWidget::setGray(int n)
{
	//m_square.fill( Qt::gray );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["grey"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["grey"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["grey"]);
}

void PeerWidget::setBlue(int n)
{
	//m_square.fill( Qt::blue );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["blue"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["blue"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["blue"]);
}

void PeerWidget::setYellow(int n)
{
	//m_square.fill( Qt::yellow );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["yellow"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["yellow"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["yellow"]);
}

void PeerWidget::setOrange(int n)
{
	//m_square.fill( QColor(255,127,0) );
	if(n == 0)
                m_statelbl->setPixmap(m_phones["orange"]);
	else if(n == 1)
                m_availlbl->setPixmap(m_persons["orange"]);
	else if(n == 2)
                m_agentlbl->setPixmap(m_agents["orange"]);
}

void PeerWidget::setAgentToolTip(const QString & tooltip)
{
        m_agentlbl->setToolTip(tooltip);
}

/*! \brief hid this widget from the panel
 */
void PeerWidget::removeFromPanel()
{
//	qDebug() << "PeerWidget::removeFromPanel()" << m_id;
	doRemoveFromPanel( m_id );
}

/*! \brief call this peer
 */
void PeerWidget::dial()
{
	qDebug() << "PeerWidget::dial()" << m_id;
        emitDial(m_id, false);
}

/*! \brief call this peer
 */
void PeerWidget::dialAgent()
{
	qDebug() << "PeerWidget::dialAgent()" << m_id;
        emitDial(m_id, true);
}

/*! \brief mouse press. store position
 */
void PeerWidget::mousePressEvent(QMouseEvent *event)
{
        // qDebug() << "PeerWidget::mousePressEvent()" << event;
	if (event->button() == Qt::LeftButton)
		m_dragstartpos = event->pos();
	//else if (event->button() == Qt::RightButton)
	//	qDebug() << "depending on what has been left-cliked on the left ...";
}

/*! \brief start drag if necessary
 */
void PeerWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_engine->isASwitchboard())
		return;
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->pos() - m_dragstartpos).manhattanLength()
	    < QApplication::startDragDistance())
		return;

	//qDebug() << "PeerWidget::mouseMoveEvent() startDrag";
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;
	qDebug() << "PeerWidget::mouseMoveEvent()" << m_id;
	mimeData->setText(m_id/*m_textlbl->text()*/);
	mimeData->setData(PEER_MIMETYPE, m_id.toAscii());
	mimeData->setData("name", m_name.toUtf8());
	drag->setMimeData(mimeData);

	/*Qt::DropAction dropAction = */
        drag->start(Qt::CopyAction | Qt::MoveAction);
	//qDebug() << "PeerWidget::mouseMoveEvent : dropAction=" << dropAction;
}

void PeerWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
        // qDebug() << "PeerWidget::mouseDoubleClickEvent" << event;
        if(event->button() == Qt::LeftButton)
                dial();
}

void PeerWidget::mouseDoubleClickEventAgent(QMouseEvent * event)
{
        // qDebug() << "PeerWidget::mouseDoubleClickEventAgent" << event;
        if(event->button() == Qt::LeftButton)
                dialAgent();
}

/*! \brief  
 *
 * filters the acceptable drag on the mime type.
 */
void PeerWidget::dragEnterEvent(QDragEnterEvent *event)
{
        // qDebug() << "PeerWidget::dragEnterEvent()" << event->mimeData()->formats();
	if(  event->mimeData()->hasFormat(PEER_MIMETYPE)
             || event->mimeData()->hasFormat(NUMBER_MIMETYPE)
             || event->mimeData()->hasFormat(CHANNEL_MIMETYPE) )
	{
		if(event->proposedAction() & (Qt::CopyAction|Qt::MoveAction))
			event->acceptProposedAction();
	}
}

/*! \brief drag move event
 *
 * filter based on the mimeType.
 */
void PeerWidget::dragMoveEvent(QDragMoveEvent *event)
{
	//qDebug() << "PeerWidget::dragMoveEvent()" << event->mimeData()->formats() << event->pos();
	event->accept(rect());
	/*if(  event->mimeData()->hasFormat(PEER_MIMETYPE)
	  || event->mimeData()->hasFormat(CHANNEL_MIMETYPE) )
	{*/
		if(event->proposedAction() & (Qt::CopyAction|Qt::MoveAction))
			event->acceptProposedAction();
	/*}*/
}

/*! \brief receive drop events
 *
 * initiate an originate or transfer
 */
void PeerWidget::dropEvent(QDropEvent *event)
{
	QString from = event->mimeData()->text();
	QString to = m_id;
        // 	qDebug() << "PeerWidget::dropEvent() :" << from << "on" << to;
        // 	qDebug() << " possibleActions=" << event->possibleActions();
        // 	qDebug() << " proposedAction=" << event->proposedAction();
        // qDebug() << "mouse & keyboard" << event->mouseButtons() << event->keyboardModifiers();
        qDebug() << "PeerWidget::dropEvent()" << event->keyboardModifiers() << event->mimeData() << event->proposedAction();

        if(event->mimeData()->hasFormat(CHANNEL_MIMETYPE)) {
                qDebug() << "CHANNEL_MIMETYPE";
        } else if(event->mimeData()->hasFormat(PEER_MIMETYPE)) {
                qDebug() << "PEER_MIMETYPE";
        } else if(event->mimeData()->hasFormat(NUMBER_MIMETYPE)) {
                qDebug() << "NUMBER_MIMETYPE";
        }

	switch(event->proposedAction()) {
	case Qt::CopyAction:
		// transfer the call to the peer "to"
	  	if(event->mimeData()->hasFormat(CHANNEL_MIMETYPE)) {
                        event->acceptProposedAction();
                        transferCall(from, to);
		} else if(event->mimeData()->hasFormat(PEER_MIMETYPE)) {
			event->acceptProposedAction();
			originateCall(from, to);
		} else if(event->mimeData()->hasFormat(NUMBER_MIMETYPE)) {
			event->acceptProposedAction();
                        originateCall(to, from);
		}
		break;
	case Qt::MoveAction:
		event->acceptProposedAction();
		atxferCall(from, to);
		break;
	default:
		qDebug() << "Unrecognized action";
		break;
	}
}

/*! \brief transfer the channel to this peer
 */
void PeerWidget::transferChan(const QString & chan)
{
	transferCall(chan, m_id);
}

/*! \brief display context menu
 */
void PeerWidget::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu contextMenu(this);
	contextMenu.addAction(m_dialAction);
	if(m_engine->isASwitchboard()) {
		// add remove action only if we are in the central widget.
		if(parentWidget() && m_engine->isRemovable(parentWidget()->metaObject()))
			contextMenu.addAction(m_removeAction);
		if( !m_channels.empty() ) {
			QMenu * interceptMenu = new QMenu( tr("&Intercept"), &contextMenu );
			QMenu * hangupMenu = new QMenu( tr("&Hangup"), &contextMenu );
			QListIterator<PeerChannel *> i(m_channels);
			while(i.hasNext()) {
				const PeerChannel * channel = i.next();
				interceptMenu->addAction(channel->otherPeer(),
							 channel, SLOT(intercept()));
				hangupMenu->addAction(channel->otherPeer(),
						      channel, SLOT(hangUp()));
			}
			contextMenu.addMenu(interceptMenu);
			contextMenu.addMenu(hangupMenu);
		}
		if( !m_mychannels.empty() ) {
			QMenu * transferMenu = new QMenu( tr("&Transfer"), &contextMenu );
			QListIterator<PeerChannel *> i(m_mychannels);
			while(i.hasNext()) {
				const PeerChannel * channel = i.next();
				transferMenu->addAction(channel->otherPeer(),
							channel, SLOT(transfer()));
			}
			contextMenu.addMenu(transferMenu);
		}
	}

	contextMenu.exec(event->globalPos());
}

/*! \brief empty m_channels
 */
void PeerWidget::clearChanList()
{
	//qDebug() << "PeerWidget::clearChanList()" << m_channels;
	//m_channels.clear();
	while(!m_channels.isEmpty())
		delete m_channels.takeFirst();
}

/*! \brief add a channel to m_channels list
 */
void PeerWidget::addChannel(const QString & id, const QString & state, const QString & otherPeer)
{
	PeerChannel * ch = new PeerChannel(id, state, otherPeer, this);
	connect(ch, SIGNAL(interceptChan(const QString &)),
	        this, SIGNAL(interceptChan(const QString &)));
	connect(ch, SIGNAL(hangUpChan(const QString &)),
	        this, SIGNAL(hangUpChan(const QString &)));
	m_channels << ch;
}

/*! \brief update calls of "ME" (for transfer context menu)
 */
void PeerWidget::updateMyCalls(const QStringList & chanIds,
                               const QStringList & chanStates,
			       const QStringList & chanOthers)
{
	while(!m_mychannels.isEmpty())
		delete m_mychannels.takeFirst();
	for(int i = 0; i<chanIds.count(); i++)
	{
		PeerChannel * ch = new PeerChannel(chanIds[i], chanStates[i], chanOthers[i]);
		connect(ch, SIGNAL(transferChan(const QString &)),
		        this, SLOT(transferChan(const QString &)) );
		m_mychannels << ch;
	}
}

/*! \brief change displayed name
 */
void PeerWidget::setName(const QString & name)
{
	m_name = name;
	m_textlbl->setText(m_name);
}

/*! \brief setter for m_engine
 *
 * set BaseEngine object to be used to connect to
 * peer object slot/signals.
 */
void PeerWidget::setEngine(BaseEngine * engine)
{
	m_engine = engine;
}
