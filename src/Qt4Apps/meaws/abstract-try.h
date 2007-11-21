#ifndef MEAWS_TRY_ABSTRACT_H
#define MEAWS_TRY_ABSTRACT_H

#include "defs.h"
#include <QObject>
#include <QVBoxLayout>
#include "QClickFrame.h"
#include "backend.h"

class Try : public QObject
{
	Q_OBJECT

public:
	Try();
	~Try();
	QFrame* getDisplay()
		{ return tryArea_; };
	void setTryNumber(mrs_natural num)
		{ tryNumber_ = num; };
	mrs_real getScore()
		{ return 0; };
	mrs_bool hasAudio()
		{ return hasAudio_; };
	void setFilename(mrs_string filename)
		{ filename_ = filename; };
	mrs_string getFilename()
		{ return filename_; };


	virtual void displayAnalysis(MarBackend *results)
		{ (void) results; hasAudio_ = true; };
	virtual void selected(bool selected)
		{ (void) selected; };

public slots:
	virtual void clicked()
	{
		emit selectTry(tryNumber_);
	};
	virtual void doubleclicked() {};

signals:
	void selectTry(mrs_natural);

protected:
	QClickFrame *tryArea_;
	QLayout *tryLayout_;

	mrs_natural tryNumber_;
	mrs_bool hasAudio_;
	mrs_string filename_;
};
#endif

