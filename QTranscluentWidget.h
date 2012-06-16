#ifndef QTRANSCLUENTWIDGET_H
#define QTRANSCLUENTWIDGET_H

#include <QtGui>

class QTranscluentWidget : public QWidget
{
	Q_OBJECT

public:
	QTranscluentWidget(QWidget *parent = 0);
	virtual ~QTranscluentWidget();

	void setOpacity(qreal opacity) { m_opacity = opacity; }

protected:
	void paintEvent(QPaintEvent * pe);

private:
	qreal m_opacity;
	
};

#endif // QTRANSCLUENTWIDGET_H
