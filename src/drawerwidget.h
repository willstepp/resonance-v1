
#ifndef DRAWERWIDGET_H
#define DRAWERWIDGET_H

#include <QImage>
#include <QWidget>
#include <QLabel>

class AeroButton;

class DrawerWidget : public QWidget
{
	Q_OBJECT

public:
	DrawerWidget(QWidget *parent);
	~DrawerWidget();

	void setParentMoved()	{ m_parent_moved = true; }

	// specify dock area
	// set constraints

	enum DrawerArea { Top, Bottom, Left, Right };
	enum DrawerState { Closed, Closing, Open, Opening };

	void setDrawerArea(DrawerArea da)	{ m_area = da; layoutChildren();		}
	DrawerArea drawerArea()				{ return m_area;						}

	void setPixmap(QPixmap p)			{ m_label->setPixmap(p); m_label->adjustSize();	}
	void setText(QString t)				{ m_label->setText(t); m_label->adjustSize();	}
	void setState(DrawerState s);
	DrawerState state()					{ return m_state; }

	void setSpeed(int s) { m_speed = s; }

	QSize thumbSize() { return m_thumb->size(); }
	QLabel *label() { return m_label; }

public slots:
	void openDrawer();
	void closeDrawer();

signals:
	void drawerMoved();

protected:
	void layoutChildren();
	virtual void moveEvent(QMoveEvent *event);
	virtual void timerEvent(QTimerEvent *event);

protected:
	AeroButton	*m_open;
	AeroButton	*m_close;

	QLabel		*m_label;
	QLabel		*m_thumb;

	DrawerArea m_area;
	DrawerState m_state;

	int m_timer;
	int m_speed;

	bool m_parent_moved;
};

#endif 

