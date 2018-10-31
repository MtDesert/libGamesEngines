#include<qt5/QtWidgets/QWidget>
#include<qt5/QtWidgets/QApplication>

class GamesWidget:public QWidget{};

int main(int argc,char* argv[]){
	QApplication app(argc,argv);
	GamesWidget widget;
	widget.show();
	return app.exec();
}