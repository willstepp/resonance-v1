#include "ResoStyle.h"

ResoStyle::ResoStyle() : QCleanlooksStyle()
{

}

ResoStyle::~ResoStyle()
{

}

void ResoStyle::drawItemText(QPainter * painter, const QRect & rectangle, int alignment, const QPalette & palette, 
				  bool enabled, const QString & text, QPalette::ColorRole textRole) const
{
	QPen pen;
	pen.setWidth(1);

	pen.setColor(Qt::black);
	painter->setPen(pen);

	QRectF right_rect = rectangle;
	QRectF bottom_rect = rectangle;
	right_rect.moveRight(rectangle.right() + 2);
	bottom_rect.moveBottom(rectangle.bottom() + 2);

	QRectF right_bottom_rect = rectangle;
	right_bottom_rect.moveBottomRight(QPoint(rectangle.right() + 2, rectangle.bottom() + 2));

	painter->drawText(right_rect, alignment, text);
	painter->drawText(bottom_rect, alignment, text);
	painter->drawText(right_bottom_rect, alignment, text);

	pen.setColor(Qt::white);
	painter->setPen(pen);

	painter->drawText(rectangle, alignment, text);
}
