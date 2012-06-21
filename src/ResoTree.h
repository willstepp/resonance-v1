#ifndef RESOTREE_H
#define RESOTREE_H

#include <QtGui>
#include <QtCore>

class Resonance;

class ResoTree : public QTreeWidget
{
	Q_OBJECT

public:
	ResoTree(QWidget * parent = 0);
	virtual ~ResoTree();

	void setSoundClass(int sound_class) { m_current_sound_class = sound_class; }

signals:
	void removeSound(QTreeWidgetItem * item);

public slots:
	void removeSoundClicked();

protected:
	void contextMenuEvent(QContextMenuEvent * ce);

private:
	QTreeWidgetItem * m_context_item;

	int m_current_sound_class;
	
};

#endif // RESOTREE_H
