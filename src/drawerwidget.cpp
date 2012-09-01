
#include <QtGui>
#include "AeroButton.h"
#include "drawerwidget.h"

DrawerWidget::DrawerWidget(QWidget *parent) : QWidget(parent)
{
	m_label = new QLabel(this);
	m_label->setLineWidth(1);
	m_label->setFrameStyle(QFrame::Box|QFrame::Raised);

	setPalette(QPalette(QColor(50, 50, 50)));

	m_thumb = new QLabel(this);

	m_open = new AeroButton("", m_thumb);
	//m_open->setColor(QColor(Qt::darkGreen));
	//m_open->setShadow(QColor(Qt::black));
	//m_open->setHighlight(QColor(Qt::green));
	QColor white = QColor(Qt::white);
	m_open->setHighlight(white);
	m_open->setRoundness(100);
	m_open->setOpacity(1.0);
	//m_open->setOutlineOpacity(0.0);
	m_open->resize(19, 19);
	connect(m_open, SIGNAL(clicked()), this, SLOT(openDrawer()));

	m_close = new AeroButton("", m_thumb);
	//m_close->setColor(QColor(Qt::darkRed));
	//m_close->setShadow(QColor(Qt::black));
	//m_close->setHighlight(QColor(Qt::red));
	m_close->setHighlight(white);
	m_close->setRoundness(100);
	m_close->setOpacity(1.0);
	//m_close->setOutlineOpacity(0.0);
	m_close->resize(19, 19);
	connect(m_close, SIGNAL(clicked()), this, SLOT(closeDrawer()));

	m_timer = 0;
	m_speed = 8;
	setDrawerArea(Top);
	openDrawer();
}

DrawerWidget::~DrawerWidget()
{
}

void DrawerWidget::layoutChildren()
{
	delete layout();

	switch(m_area)
	{
		case Left:
		{
			QPixmap p = QPixmap("images/thumb.png");
			m_thumb->setPixmap(p);
			m_thumb->setFixedSize(p.size());
			m_thumb->setAlignment(Qt::AlignCenter);
			m_thumb->setText("Properties");

			QHBoxLayout *h = new QHBoxLayout(this);
			QVBoxLayout *v = new QVBoxLayout();

			v->setSpacing(0);
			v->setContentsMargins(0, 0, 0, 0);
			h->setSpacing(0);
			h->setContentsMargins(0, 0, 0, 0);
			h->addWidget(m_label);
			h->addLayout(v);

			v->addStretch();
			v->addWidget(m_thumb);
			v->addStretch();

			m_open->move(5, 92);
			m_close->move(5, 92);

			m_open->setIcon(QIcon("images/down.png"));
			m_close->setIcon(QIcon("images/up.png"));

			setLayout(h);
		}
		break;

		case Right:
		{
			QPixmap p = QPixmap::fromImage(QImage("images/thumb.png").mirrored(true, false));
			m_thumb->setPixmap(p);
			m_thumb->setFixedSize(p.size());

			QHBoxLayout *h = new QHBoxLayout(this);
			QVBoxLayout *v = new QVBoxLayout();

			v->setSpacing(0);
			v->setContentsMargins(0, 0, 0, 0);
			h->setSpacing(0);
			h->setContentsMargins(0, 0, 0, 0);
			h->addLayout(v);

			v->addStretch();
			v->addWidget(m_thumb);
			v->addStretch();

			h->addWidget(m_label);

			m_open->move(6, 92);
			m_close->move(6, 92);

			m_open->setIcon(QIcon("images/left.png"));
			m_close->setIcon(QIcon("images/right.png"));

			setLayout(h);
		}
		break;

		case Top:
		{
			//QPixmap p = QPixmap::fromImage(QImage("images/hthumb.png").mirrored());
			//m_thumb->setPixmap(p);
			m_thumb->setFixedSize(QSize(119, 31));
			m_thumb->setAlignment(Qt::AlignCenter);
			m_thumb->setText("Modules");
			m_thumb->hide();

			QVBoxLayout *v = new QVBoxLayout(this);
			QHBoxLayout *h = new QHBoxLayout();

			v->setSpacing(0);
			v->setContentsMargins(0, 0, 0, 0);
			h->setSpacing(0);
			h->setContentsMargins(0, 0, 0, 0);
			v->addWidget(m_label);
			v->addLayout(h);

			h->addStretch();
			h->addWidget(m_thumb);
			h->addStretch();

			m_open->move(86, 7);
			m_close->move(86, 7);

			m_open->setIcon(QIcon("images/down.png"));
			m_open->setIconSize(QSize(7, 5));
			m_open->hide();
			m_close->setIcon(QIcon("images/up.png"));
			m_close->setIconSize(QSize(7, 5));
			m_close->hide();

			setLayout(v);
		}
		break;

		case Bottom:
		{
			QPixmap p = QPixmap::fromImage(QImage("images/hthumb.png"));
			m_thumb->setPixmap(p);
			m_thumb->setFixedSize(p.size());

			QVBoxLayout *v = new QVBoxLayout(this);
			QHBoxLayout *h = new QHBoxLayout();

			v->setSpacing(0);
			v->setContentsMargins(0, 0, 0, 0);
			h->setSpacing(0);
			h->setContentsMargins(0, 0, 0, 0);

			v->addLayout(h);
			v->addWidget(m_label);

			h->addStretch();
			h->addWidget(m_thumb);
			h->addStretch();

			m_open->move(92, 5);
			m_close->move(92, 5);

			m_open->setIcon(QIcon("images/up.png"));
			m_close->setIcon(QIcon("images/down.png"));

			setLayout(v);
		}
		break;

	}
}

void DrawerWidget::openDrawer()
{
	setState(Opening);
	if(!m_timer)
		m_timer = startTimer(10);
}

void DrawerWidget::closeDrawer()
{
	setState(Closing);
	if(!m_timer)
		m_timer = startTimer(10);
}

void DrawerWidget::timerEvent(QTimerEvent * /*event*/)
{
	if(Closing == m_state)
	{
		switch(m_area)
		{
		case Left:
			move(x() - m_speed, y());
			break;
		case Right:
			move(x() + m_speed, y());
			break;
		case Top:
			move(x(), y() - m_speed);
			break;
		case Bottom:
			move(x(), y() + m_speed);
			break;
		}
	}
	else if(Opening == m_state)
	{
		switch(m_area)
		{
		case Left:
			move(x() + m_speed, y());
			break;
		case Right:
			move(x() - m_speed, y());
			break;
		case Top:
			move(x(), y() + m_speed);
			break;
		case Bottom:
			move(x(), y() - m_speed);
			break;
		}
	}
	else 
	{
		killTimer(m_timer);
		m_timer = 0;
	}
}

void DrawerWidget::setState(DrawerState s)
{
	m_state = s;

	if(Closing == s || Closed == s)
	{
		m_open->setVisible(true);
		m_close->hide();
	}

	if(Opening == s || Open == s)
	{
		m_close->setVisible(true);
		m_open->hide();
	}

	if(Open == s || Closed == s)
	{
		killTimer(m_timer);
		m_timer = 0;
		emit drawerMoved();
	}
}

void DrawerWidget::moveEvent(QMoveEvent *event)
{
	switch(m_area)
	{
		case Left:
		{
			if(event->pos().x() > 0)
			{
				move(0, event->pos().y());
				setState(Open);
			}

			if(event->pos().x() < -width() + m_thumb->width())
			{
				move(-width() + m_thumb->width(), event->pos().y());
				setState(Closed);
			}
		}
		break;

		case Right:
		{
			if(event->pos().x() < parentWidget()->width() - width())
			{
				move(parentWidget()->width() - width(), event->pos().y());
				setState(Open);
			}

			if(event->pos().x() > parentWidget()->width() - m_thumb->width())
			{
				move(parentWidget()->width() - m_thumb->width(), event->pos().y());
				setState(Closed);
			}
		}
		break;

		case Top:
		{
				if(event->pos().y() > 0)
				{
					move(event->pos().x(), 0);
					setState(Open);
				
				}
				if(event->pos().y() < -height() + m_thumb->height() + 0)
				{
					move(event->pos().x(), -height() + m_thumb->height() + 0);
					setState(Closed);
				}
		}
		break;

		case Bottom:
		{
			if(event->pos().y() > parentWidget()->height() - m_thumb->height() - 0)
			{
				move(event->pos().x(), parentWidget()->height() - m_thumb->height() - 0);
				setState(Closed);
			}

			if(event->pos().y() < parentWidget()->height() - height())
			{
				move(event->pos().x(), parentWidget()->height() - height());
				setState(Open);
			}
		}
		break;
	}

	emit drawerMoved();
}
