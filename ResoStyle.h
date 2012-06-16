#ifndef RESOSTYLE_H
#define RESOSTYLE_H

#include <QtGui>

class ResoStyle : public QCleanlooksStyle
{
	Q_OBJECT

public:
	ResoStyle();
	virtual ~ResoStyle();

	void drawItemText(QPainter * painter, const QRect & rectangle, int alignment, const QPalette & palette, 
		bool enabled, const QString & text, QPalette::ColorRole textRole = QPalette::NoRole) const;

private:
	
};

#endif // RESOSTYLE_H
