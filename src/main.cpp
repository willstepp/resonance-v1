#include <QtGui/QApplication>
#include "Resonance.h"
#include "ResoModule.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Resonance resonance;
	resonance.show();
	return a.exec();
}
