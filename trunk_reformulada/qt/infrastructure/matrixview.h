/***************************************************************************
 *   E-Foto Project: Digital Photogrammetric Workstation                   *
 *   Rio de Janeiro State University                                       *
 *   Program of Post-Graduation in Geomatics                               *
 *                                                                         *
 *   Copyright (C) 2010 by E-Foto Project                                  *
 *                                                                         *
 *   Developers:           Guilherme Lucio Abelha Mota                     *
 *                         Marcos Vinícius Meffe                           *
 *                                                                         *
 *   contact:  guimota at ime.uerj.br                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H 

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class MatrixView;

class MatrixModel : public AbstractEModel  
{
friend class MatrixView;

public:

    MatrixModel(vector <string> StrMatrix);
    MatrixModel(unsigned int prows, unsigned int pcols,unsigned int pviewheader);
	void fillvalues(vector <string> StrMatrix);
	string strMatrix;
	string getvalues();
	void showModel(void);

private:

	unsigned int rows,columns,viewheader;

protected:
	void putVertHeader(void);
};

class MatrixLayout : public QVBoxLayout
{
    Q_OBJECT

friend class MatrixView;

public:
    MatrixLayout(QWidget *,int viewheaderlayout);
    ~MatrixLayout();
    

private:      
      QTableView *matrixTable;
      
 //     QHeaderView *headerTable;
      
      int viewheaderlayout;
      
};


class MatrixView : public QWidget
{
      Q_OBJECT
      
public:
    MatrixView(QWidget *parent = 0,  MatrixModel* matrixmodel=0);
    ~MatrixView();
    
private:
    MatrixLayout *matrixlayout;
};

#endif
