/*
 * Copyright (C) 2011 Weill Medical College of Cornell University
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "IS_DC_Protocol.h"
#include <iostream>

#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#else
	#include <QtGui>
#endif

using namespace std;

/* AddStepInputDialog Class */
AddStepInputDialog::AddStepInputDialog(QWidget* parent ) : AddStepDialog( parent ) /*, 0, TRUE )*/ {
//	QValidator* validator = new QDoubleValidator(this);
//	BCLEdit->setValidator(validator);
//	DIEdit->setValidator(validator);
//	numBeatsEdit->setValidator(validator);
//	scalingPercentageEdit->setValidator(validator);

	QObject::connect( addStepButton,SIGNAL(clicked(void)),this,SLOT(addStepClicked(void)) );
	QObject::connect( exitButton, SIGNAL(clicked(void)), this, SLOT( reject() ) );
	
	// Dialog returns Accept after inputs have been checked
	QObject::connect( this, SIGNAL(checked(void)), this, SLOT(accept()) ); 

	// Updates when combo box selection is changed
	QObject::connect( stepComboBox, SIGNAL(activated(int)), SLOT(stepComboBoxUpdate(int)) ); 

	stepComboBoxUpdate(0);
}

AddStepInputDialog::~AddStepInputDialog( void ) { }

void AddStepInputDialog::stepComboBoxUpdate( int selection ) {

	switch( (ProtocolStep::stepType_t)selection ) {
	
	case ProtocolStep::PACE:
		BCLEdit->setEnabled(true);
		numBeatsEdit->setEnabled(true);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::SCALE:
		BCLEdit->setEnabled(true);
		numBeatsEdit->setEnabled(true);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(true);
		scalingPercentageEdit->setEnabled(true);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::WAIT:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(false);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(true);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::DIPACE:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(true);
		DIEdit->setEnabled(true);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::DISCALE:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(true);
		DIEdit->setEnabled(true);
		currentToScaleEdit->setEnabled(true);
		scalingPercentageEdit->setEnabled(true);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::STARTMODEL:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(false);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::STOPMODEL:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(false);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::RESETMODEL:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(false);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(false);
		break;

	case ProtocolStep::CHANGEMODEL:
		BCLEdit->setEnabled(false);
		numBeatsEdit->setEnabled(false);
		DIEdit->setEnabled(false);
		currentToScaleEdit->setEnabled(false);
		scalingPercentageEdit->setEnabled(false);
		waitTimeEdit->setEnabled(false);
		modelComboBox->setEnabled(true);
		break;
	}
}

// Initializes QStrings and checks if they are valid entries
void AddStepInputDialog::addStepClicked( void ) { 
	bool check = true;
	BCL = BCLEdit->text();
	DI = DIEdit->text();
	stepType = QString::number( stepComboBox->currentIndex() );
	numBeats = numBeatsEdit->text();
	currentToScale = currentToScaleEdit->text();
	scalingPercentage = scalingPercentageEdit->text();
	waitTime = waitTimeEdit->text();
	model = QString::number( modelComboBox->currentIndex() );

	switch( stepComboBox->currentIndex() ) {
	
	case ProtocolStep::PACE: // Pace
		if (BCL == "" || numBeats == "") check = false;
		break;

	case ProtocolStep::SCALE: // Scale
		if (BCL == "" || numBeats == "" || currentToScale == "") check = false;
		break;

	case ProtocolStep::WAIT: // Wait
		if (waitTime == "") check = false;
		break;

	case ProtocolStep::DIPACE: // DI
		if (DI == "" || numBeats == "") check = false;
		break;

	case ProtocolStep::DISCALE: // DI + Scaling
		if (BCL == "" || numBeats == "" || currentToScale == "") check = false;
		break;

	case ProtocolStep::CHANGEMODEL: // Change Model
		if (model == "") check = false;
		break;

	default: 
		std::cout<<"default called in addStepClicked() - something went horribly wrong."<<std::endl;
		break;
	}

	if (check) emit checked();
	else QMessageBox::warning( this, "Error", "Invalid Input, please correct." );
}

vector<QString> AddStepInputDialog::gatherInput( void ) {
	std::vector<QString> inputAnswers;

	if( exec() == QDialog::Rejected ) {
		// Return an empty vector if step window is closed
		return inputAnswers; 
	}
	else { 
		// QDialog is accepted when addStep button is pressed and inputs are considered valid
		inputAnswers.push_back( QString::number( stepComboBox->currentIndex() ) );
		//inputAnswers.push_back( stepType );
		inputAnswers.push_back( BCL );
		inputAnswers.push_back( DI );
		inputAnswers.push_back( numBeats );
		inputAnswers.push_back( currentToScale );
		inputAnswers.push_back( scalingPercentage );
		inputAnswers.push_back( waitTime );
		//inputAnswers.push_back( model );
		inputAnswers.push_back( QString::number( modelComboBox->currentIndex() ) );
		return inputAnswers;
	}
}

/* Protocol Step Class */
ProtocolStep::ProtocolStep( stepType_t st, double bcl, double di, int nb, 
                            string c, int sp, int w, modelType_t(mt) ) :
                            stepType(st), BCL(bcl), DI(di), numBeats(nb), 
                            currentToScale(c), scalingPercentage(sp), 
                            waitTime(w), modelType(mt) { }

ProtocolStep::~ProtocolStep( void ) { }

int ProtocolStep::stepLength( double period ) {
	return 0;
}

/* Protocol Class */
Protocol::Protocol( void ) { }

Protocol::~Protocol( void ) { }

// Opens input dialogs to gather step information, then adds to protocol 
// container *at the end*. Returns true if a step was added
bool Protocol::addStep( QWidget *parent ) {
	// Special dialog box for step parameter input
	AddStepInputDialog *dlg = new AddStepInputDialog(parent);
	
	// Opens dialog box for input
	vector<QString> inputAnswers = dlg->gatherInput(); 
	delete dlg;

	if( inputAnswers.size() > 0 ) {
		// Add a new step to protocol container
		protocolContainer.push_back( 
			ProtocolStepPtr( new ProtocolStep (
			(ProtocolStep::stepType_t)( inputAnswers[0].toInt() ), // stepType
			inputAnswers[1].toDouble(), // BCL
			inputAnswers[2].toDouble(), // DI
			inputAnswers[3].toInt(), // numBeats
			inputAnswers[4].toStdString(), // currentToScale
			inputAnswers[5].toInt(), // scalingPercentage
			inputAnswers[6].toInt(), // waitTime
			(ProtocolStep::modelType_t)( inputAnswers[7].toInt() ) // model
		) ) );
		return true;
	}
	else {
		return false; // No step added
	}
}

// Opens input dialogs to gather step information, then adds to protocol 
// container at *a specific point*. Returns true if a step was added
bool Protocol::addStep( QWidget *parent, int idx ) {
	// Special dialog box for step parameter input
	AddStepInputDialog *dlg = new AddStepInputDialog(parent); 

	// Opens dialog box for input
	vector<QString> inputAnswers = dlg->gatherInput(); 
	delete dlg;

	vector<ProtocolStepPtr>::iterator it = protocolContainer.begin();

	if( inputAnswers.size() > 0 ) {
		// Add a new step to protocol container
		protocolContainer.insert( 
			it+idx+1, ProtocolStepPtr( 
				new ProtocolStep(
					(ProtocolStep::stepType_t)( inputAnswers[0].toInt() ), //stepType
					inputAnswers[1].toDouble(), // BCL
					inputAnswers[2].toDouble(), // DI
					inputAnswers[3].toInt(), // numBeats
					inputAnswers[4].toStdString(), // currentToScale
					inputAnswers[5].toInt(), // scalingPercentage
					inputAnswers[6].toInt(), // waitTime
					(ProtocolStep::modelType_t)( inputAnswers[6].toInt() ) //model
				) 
			) 
		);
		return true;
	}
	else return false; // No step added
}

// Deletes a step
void Protocol::deleteStep( QWidget *parent, int stepNumber ) {
	// Message box asking for confirmation whether step should be deleted
	QString text = "Do you wish to delete step " + QString::number(stepNumber+1) + "?"; 
	
	// text pointing out specific step
	if( QMessageBox::question(parent, "Delete Step Confirmation",
	                                 text, "Yes", "No")) {
		return ; // Answer is no
	}

	// If only 1 step exists, explicitly clear step pointer
	if( protocolContainer.size() == 1 ) protocolContainer.clear();
	else protocolContainer.erase(protocolContainer.begin() + stepNumber);
}

void Protocol::saveProtocol( QWidget *parent ) {
	// Make sure protocol has at least one segment with one step
	if( protocolContainer.size() == 0 ) { 
		QMessageBox::warning(parent, "Error",
		                     "A protocol must contain at least one step");
		return;
	}

	// Create QDomDocument
	QDomDocument protocolDoc("IScaleProtocol");
	QDomElement root = protocolDoc.createElement( "IS_DC_protocol-v1.0");
	protocolDoc.appendChild(root);   

	// Save dialog to retrieve desired filename and location
	QString fileName = QFileDialog::getSaveFileName(parent, "Save protocol", 
	                                                "~/", "XML Files (*.xml)");

	// If filename does not include .xml extension, add extension
	if( !(fileName.endsWith(".xml")) ) fileName.append(".xml");

	// If filename exists, warn user
	if ( QFileInfo(fileName).exists() &&
		QMessageBox::warning( 
			parent, "File Exists", "Do you wish to overwrite " + fileName + "?",
			QMessageBox::Yes | QMessageBox::Default, 
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes) {
		return ; // Return if answer is no
	}

	// Add segment elements to protocolDoc
	for( int i = 0; i < protocolContainer.size(); i++ ) {
		root.appendChild( stepToNode(protocolDoc, protocolContainer.at(i), i) );
	}

	// Save protocol to file
	QFile file(fileName); 

	// Open file, return error if unable to do so
	if( !file.open(QIODevice::WriteOnly) ) {
		QMessageBox::warning(
			parent, "Error", 
			"Unable to save file: Please check folder permissions.");
		return ;
	}

	QTextStream ts(&file); // Open text stream
	ts << protocolDoc.toString(); // Write to file
	file.close(); // Close file
}

QString Protocol::loadProtocol( QWidget *parent ) {
	// If protocol is present, warn user that protocol will be lost upon loading
	if( protocolContainer.size() != 0 &&
		QMessageBox::warning(parent,
			"Load Protocol", 
			"All unsaved changes to current protocol will be lost.\nDo you wish to continue?",
			QMessageBox::Yes | QMessageBox::Default, 
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes ) {
		return ""; // Return if answer is no
	}

	// Save dialog to retrieve desired filename and location
	QString fileName = QFileDialog::getOpenFileName(parent, "Open a protocol", 
	                                                "~/","XML Files (*.xml)");
	QDomDocument doc( "IS_DC_Protocol" );
	QFile file( fileName );

	// Make sure file can be opened, if not, warn user
	if( !file.open( QIODevice::ReadOnly ) ) { 
		QMessageBox::warning(parent, "Error", "Unable to open protocol file" );
		return "";
	}   

	// Make sure file contents are loaded into document
	if( !doc.setContent( &file ) ) { 
		QMessageBox::warning(parent, "Error", 
		                     "Unable to set file contents to document");
		file.close();
		return "";
	}

	file.close();

	QDomElement root = doc.documentElement(); // Get root element from document
	
	// Check if tagname is correct for this module version
	if( root.tagName() != "IS_DC_protocol-v1.0" ) { 
		QMessageBox::warning(parent, "Error", "Incompatible XML file" );
		return "";
	}

	// Retrieve information from document and set to protocolContainer
	QDomNode stepNode = root.firstChild(); // Retrieve first segment
	protocolContainer.clear(); // Clear vector containing protocol

	while( !stepNode.isNull() ) { // Step iteration
		QDomElement stepElement = stepNode.toElement();

		// Add new step to protocol container
		protocolContainer.push_back( 
			ProtocolStepPtr( new ProtocolStep(
				(ProtocolStep::stepType_t)stepElement.attribute("stepType").toInt(),
				stepElement.attribute( "BCL" ).toDouble(),
				stepElement.attribute( "DI" ).toDouble(),
				stepElement.attribute( "numBeats" ).toInt(),
				stepElement.attribute( "currentToScale" ).toStdString(),
				stepElement.attribute( "scalingPercentage" ).toInt(),
				stepElement.attribute( "waitTime" ).toInt(),
				(ProtocolStep::modelType_t)stepElement.attribute("modelType").toInt()
		) ) ); // Add step to segment container            

		stepNode = stepNode.nextSibling(); // Move to next step
	} // End step iteration

	// Update segment summary and table
	if( protocolContainer.size() == 0 ) {
		QMessageBox::warning(parent, "Error", "Protocol did not contain any steps" );
	}

	return fileName;
}

// Load protocol straight from designated filename, used for settings file loading
void Protocol::loadProtocol( QWidget *parent, QString fileName ) {
	// If protocol is present, warn user that protocol will be lost upon loading
	if( protocolContainer.size() != 0 &&
		QMessageBox::warning( parent,
			"Load Protocol", 
			"All unsaved changes to current protocol will be lost.\nDo you wish to continue?",
			QMessageBox::Yes | QMessageBox::Default, 
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes )
		return ; // Return if answer is no

	QDomDocument doc( "IS_DC_Protocol" );
	QFile file( fileName );

	if( !file.open( QIODevice::ReadOnly ) ) { // Make sure file can be opened, if not, warn user
		QMessageBox::warning(parent, "Error", "Unable to open protocol file" );
		return ;
	}

	if( !doc.setContent( &file ) ) { // Make sure file contents are loaded into document
		QMessageBox::warning(parent, "Error", "Unable to set file contents to document" );
		file.close();
		return ;
	}
	file.close();

	QDomElement root = doc.documentElement(); // Get root element from document

	// Check if tagname is correct for this module version
	if( root.tagName() != "IS_DC_protocol-v1.0" ) { 
		QMessageBox::warning(parent, "Error", "Incompatible XML file" );
		return;
	}

	// Retrieve information from document and set to protocolContainer
	QDomNode stepNode = root.firstChild(); // Retrieve first segment
	protocolContainer.clear(); // Clear vector containing protocol

	while( !stepNode.isNull() ) {// Step iteration
		QDomElement stepElement = stepNode.toElement();

		// Add new step to protocol container
		protocolContainer.push_back( 
			ProtocolStepPtr( new ProtocolStep(
				(ProtocolStep::stepType_t)stepElement.attribute("stepType").toInt(),
				stepElement.attribute( "BCL" ).toDouble(),
				stepElement.attribute( "DI" ).toDouble(),
				stepElement.attribute( "numBeats" ).toInt(),
				stepElement.attribute( "currentToScale" ).toStdString(),
				stepElement.attribute( "scalingPercentage" ).toInt(),
				stepElement.attribute( "waitTime" ).toInt(),
				(ProtocolStep::modelType_t)stepElement.attribute("modelType").toInt()
		) ) ); // Add step to segment container            

		stepNode = stepNode.nextSibling(); // Move to next step
	} // End step iteration

	// Update segment summary and table
	if( protocolContainer.size() == 0 ) {
	QMessageBox::warning(parent, "Error", "Protocol did not contain any steps" );
	}
}

QDomElement Protocol::stepToNode( QDomDocument &doc, const ProtocolStepPtr stepPtr, int stepNumber ) {
	QDomElement stepElement = doc.createElement("step"); // Step element

	// Set attributes of step to element
	stepElement.setAttribute( "stepType", QString::number( stepPtr->stepType ) );
	stepElement.setAttribute( "BCL", QString::number( stepPtr->BCL ) );
	stepElement.setAttribute( "DI", QString::number( stepPtr->DI ) );
	stepElement.setAttribute( "numBeats", QString::number( stepPtr->numBeats ) );
	stepElement.setAttribute( "currentToScale", QString::fromStdString( stepPtr->currentToScale ) );
	stepElement.setAttribute( "scalingPercentage", stepPtr->scalingPercentage );
	stepElement.setAttribute( "waitTime", stepPtr->waitTime );
	stepElement.setAttribute( "modelType", stepPtr->modelType );

	return stepElement;
}

void Protocol::clearProtocol( void ) {
	protocolContainer.clear();
}

QString Protocol::getStepDescription( int stepNumber ) {
	ProtocolStepPtr step = protocolContainer.at( stepNumber );

	QString type;
	QString description;

	switch( step->stepType ) {

	case ProtocolStep::PACE:
		type = "Pace ";
		description = type + ": " + QString::number( step->numBeats ) + 
		              " beats - " + QString::number( step->BCL ) + "ms BCL";
		break;

	case ProtocolStep::DIPACE:
		type = "DI Pace ";
		description = type + ": " + QString::number( step->numBeats ) + 
		              " beats - " + QString::number( step->DI ) + "ms DI";
		break;

	case ProtocolStep::SCALE:
		type = "Scale " + QString::fromStdString(step->currentToScale) + "(" + 
		       QString::number( step->scalingPercentage ) + "%)";
		description = type + ": " + QString::number( step->numBeats ) + 
		              " beats - " + QString::number( step->BCL ) + "ms BCL";
		break;
	
	case ProtocolStep::DISCALE:
		type = "DI Scale " + QString::fromStdString(step->currentToScale) + "(" + 
		       QString::number( step->scalingPercentage ) + "%)";
		description = type + ": " + QString::number( step->numBeats ) + 
		              " beats - " + QString::number( step->DI ) + "ms DI";
		break;

	case ProtocolStep::WAIT:
		type = "Wait ";
		description = type + " : " + QString::number( step->waitTime ) + "ms";
		break;

	case ProtocolStep::STARTMODEL:
		description = "Start Model Cell Voltage Clamp";
		break;

	case ProtocolStep::STOPMODEL:
		description = "Stop Model Cell Voltage Clamp";
		break;

	case ProtocolStep::RESETMODEL:
		description = "Reset Model Cell to Initial Conditions";
		break;

	case ProtocolStep::CHANGEMODEL:
		QString model;
		
		switch( step->modelType ) {
	
		case ProtocolStep::LIVRUDY2009:
			model = "Livshitz Rudy 2009";
			break;
	
		case ProtocolStep::FABERRUDY2000:
			model = "Faber Rudy 2000";
			break;
		}
		description = "Change Model to (" + model + ")";
		break;

	}

	return description;
}
