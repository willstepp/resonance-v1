#include "QTranscluentWidget.h"

QTranscluentWidget::QTranscluentWidget(QWidget *parent) : QWidget(parent), m_opacity(0.9)
{

}

QTranscluentWidget::~QTranscluentWidget()
{

}

void QTranscluentWidget::paintEvent(QPaintEvent * pe)
{
	Q_UNUSED(pe);
	QPainter painter(this);
	painter.setOpacity(m_opacity);
	painter.fillRect(rect(), QBrush(Qt::black));
}
