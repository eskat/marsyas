/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef MARSYAS_SVMCLASSIFIER_H
#define MARSYAS_SVMCLASSIFIER_H

#include "MarSystem.h"
#include "svm.h" 
#include "WekaData.h" 

namespace Marsyas
{
/**
	\class SVMClassifier
	\ingroup MachineLearning
	\brief SVM classifier based on libsvm

*/

class SVMClassifier: public MarSystem
{
private:
	void addControls();
	void myUpdate(MarControlPtr sender);

	MarControlPtr ctrl_gain_EXAMPLE_;
	WekaData instances_;
	struct svm_problem svm_prob_;
	struct svm_parameter svm_param_;
	struct svm_model *svm_model_;
	mrs_string mode_, prev_mode_;

public:
	SVMClassifier(std::string name);
	SVMClassifier(const SVMClassifier& a);
	~SVMClassifier();
	MarSystem* clone() const;

	void myProcess(realvec& in, realvec& out);
};

}

#endif

