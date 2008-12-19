
#ifndef GRID_H
#define GRID_H

#include <ostream>

#include <QList>
#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QStringList>

#include <ctime>
#include <cstdlib>
#include <map>

#include "../Interface/MyDisplay.h"
#include "../Interface/Classifier.h"
#include "../Interface/Extractor.h"
#include "MarSystemManager.h"
#include "MarSystemQtWrapper.h"
#include "Collection.h"

#include "../Music/MusicCollection.h"
#include "../Colormaps/Colormap.h"

#include "../Widgets/Tracklist.h"

#define GRID_TRAINING_FILE	"training.mf"
#define GRID_PREDICTION_FILE	"predict.mf"
#define NUM_GENRES 10

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QPainter;

class Classifier;
class Extractor;
class MarsyasECP;
class GridTriplet;


using namespace MarsyasQt;

class Grid : public QThread
{
	Q_OBJECT

public:
	Grid();
	~Grid();
	void clear();
	void run();
	void addTrack(int x, int y, QString track);
	void setGridCounter(int index, int value);
	void playTrack(int counter);
	void stopPlaying();
	void removeInitFile();

	QVector< QList<std::string> > getFiles() const { return files_; }
	QList<std::string> getFilesAt(int index);
	QList<std::string> getCurrentFiles();
	QWaitCondition buttonPressed;

	void setXPos(int value);
	void setYPos(int value);
	void setXGridSize(QString);
	void setYGridSize(QString);
	void setPlaylist(std::string playlist);
	void addInitFile(QString fileName, int x, int y);
	void setContinuous(bool value);
	bool isContinuous() const {return continuous_; }
	int getGridCounterSizes(int index);
	int getGridCounter(int index);
	int getCurrentIndex();
	QList<std::string> getInitFiles();
	int * getDensity(int index);
	int getHeight() const { return som_height; }
	int getWidth() const { return som_width; }
	int getXPos() const { return _gridX; }
	int getYPos() const { return _gridY; }
	int getCellSize(int winSize);

	public slots: 
		void setExtractMode();
		void setTrainMode();
		void setPredictMode();
		void setInitMode();
		void clearMode();
		void openPredictionGrid(QString fname);
		void savePredictionGrid(QString fname);
		void cancelPressed();
		void openHash();
		void resetGridSlot();

signals: 
		void playingTrack(MusicTrack *track);
		void repaintSignal();
		void errorBox(QString msg);

protected:
	void resetGrid();

	void setGridX(int x);  
	void setGridY(int y); 

	void setup();
	void setupNetworks();
	void extract();
	void predict();
	void train();
	void train(bool skipTraining);
	void init();
	void extractAction(std::string filename);


private:
	int _winSize;
	int _cellSize;
	int _gridX;
	int _gridY;
	int som_height;
	int som_width;
	int oldWidth_;
	int oldHeight_;
	int grid_x;
	int grid_y;
	int state_; // 0 for none, 1 for extract, 2 for train, 3 for predict
	int oldPlayingIndex;
	int numFeatures;
	int **genreDensity;
	bool initAudio_;
	bool continuous_;
	bool cancel_;
	bool init_;
	std::string playlist_;



	// Indexed with index = (y * som_hight + x)
	QVector<QList <std::string> > files_;
	QVector<QList <GridTriplet*> > initFileLocations;
	QMutex mutex;

	multimap<string, realvec>* featureHash;
	multimap<string, realvec>* normFeatureHash;
	MusicCollection *_collection;

	MarControlPtr filePtr_;

	Marsyas::MarSystemManager mng;  
	QVector<int> counters;
	QVector<int> counterSizes;
	QVector<int> labels;

	MarSystemQtWrapper*  mwr_;
	Marsyas::MarSystem* pnet_;

	Marsyas::realvec _normSomFmatrix;
	Marsyas::MarSystem* som_;
	Marsyas::MarSystem* total_;
	Marsyas::MarSystem* norm_;

};

/*
Used to store info about files being used to initialize including the x,y location on the grid
and the file's absolute file path
*/
class GridTriplet
{
public:
	GridTriplet();
	GridTriplet(int, int);
	GridTriplet(std::string, int,int);
	int getX() const {return x;}
	int getY() const {return y;}
	std::string getFileName() const {return fileName;}

	void setX(int);
	void setY(int);
	void setFileName(std::string);


private:
	int x;
	int y;
	std::string fileName;
};

#endif
