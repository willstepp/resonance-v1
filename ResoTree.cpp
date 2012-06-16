#include "ResoTree.h"

#include "Resonance.h"

ResoTree::ResoTree(QWidget * parent) : QTreeWidget(parent), m_current_sound_class(0)
{

}

ResoTree::~ResoTree()
{

}

void ResoTree::contextMenuEvent(QContextMenuEvent * ce)
{
	m_context_item = itemAt(ce->pos());
	if (m_context_item)
	{
		if(m_current_sound_class == Resonance::Personal)
		{
			QMenu menu(this);
			menu.setStyle(new QCleanlooksStyle());
			QAction action("remove", &menu);
			menu.addAction(&action);
			connect(&action, SIGNAL(triggered()), this, SLOT(removeSoundClicked()));
			menu.exec(ce->globalPos());
		}
	}
}

void ResoTree::removeSoundClicked()
{
	emit removeSound(m_context_item);
}
