#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"


int main( int argc, char **argv )
{
	
	int WindowWidth = 340;
	int WindowHeight = 290;
	int ImageWidth = 320;
	int ImageHeight = 240;

	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(400, 300, WindowWidth, WindowHeight);

	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(ImageWidth, ImageHeight, QImage::Format_RGB888);
	
	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(10, 10, ImageWidth, ImageHeight);
	myLabel.setPixmap(QPixmap::fromImage(myImage));

	int numberOfButtons = 4;
	//create a FFC button
	QPushButton *button1 = new QPushButton("FFC", myWidget);
	button1->setGeometry(ImageWidth/numberOfButtons-100, WindowHeight-35, 100, 30);
	
	//create a Snapshot button
	QPushButton *button2 = new QPushButton("Capture", myWidget);
	button2->setGeometry(ImageWidth/numberOfButtons+10, WindowHeight-35, 100, 30);
	
	//create a reset button
	QPushButton *button3 = new QPushButton("Restart", myWidget);
	button3->setGeometry(ImageWidth/numberOfButtons+120, WindowHeight-35, 100, 30);

	//create a disable AGC button
	QPushButton *button4 = new QPushButton("Disable AGC", myWidget);
	button3->setGeometry(ImageWidth/numberOfButtons+120, WindowHeight-35, 100, 30);

	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));
	
	//connect ffc button to the thread's ffc action
	QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));
	//connect snapshot button to the thread's snapshot action
	QObject::connect(button2, SIGNAL(clicked()), thread, SLOT(snapshot()));
	//connect restart button to the thread's restart action
	QObject::connect(button3, SIGNAL(clicked()), thread, SLOT(restart()));

	//connect restart button to the thread's restart action
	QObject::connect(button4, SIGNAL(clicked()), thread, SLOT(disable_agc()));

	thread->start();
	
	myWidget->show();

	return a.exec();
}

