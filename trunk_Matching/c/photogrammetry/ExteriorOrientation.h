/**************************************************************************
                        ExteriorOrientation.h
**************************************************************************/


#ifndef EXTERIORORIENTATION_H
#define EXTERIORORIENTATION_H

#include "EObject.h"

class Image;

/**
  * class ExteriorOrientation
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class ExteriorOrientation : public EObject
{
    //EOBJECT

protected:

    // Associated objects
    //
    int imageId;
    Image* myImage;

public:

    // Associated objects accessor methods
    //
    void setImageId(int newImageId);
    void setImage(Image* newImage);
    int getImageId();
    Image* getImage();

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);


};

#endif //EXTERIORORIENTATION_H
