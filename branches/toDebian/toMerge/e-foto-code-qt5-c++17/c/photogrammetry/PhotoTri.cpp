/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "PhotoTri.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

PhotoTri::PhotoTri()
{

}

std::string PhotoTri::xmlGetData()
{
    // todo: make xml output
    return "";
}

void PhotoTri::xmlSetData(std::string xml)
{
    EDomElement root(xml);

    totalIterations = root.elementByTagName("iterations").toInt();
    converged = root.elementByTagName("converged").toString() == "true";
    metricConvergency = root.elementByTagName("metricConvergency").toDouble();
    angularConvergency = root.elementByTagName("angularConvergency").toDouble();
    rmse = root.elementByTagName("rmse").toDouble();

    std::deque<EDomElement> pts = root.elementByTagName("usedPoints").children();
    for (unsigned int i = 0; i <pts.size(); i++)
        pointKey.push_back(pts.at(i).elementByTagName("pointKey").toInt());

    std::deque<EDomElement> imgs = root.elementByTagName("usedImages").children();
    for (unsigned int i = 0; i <imgs.size(); i++)
        imageKey.push_back(imgs.at(i).elementByTagName("imageKey").toInt());

}

void PhotoTri::putImage(Image *img)
{
    bool insert = true;
    // Eliminamos primeiro a possibilidade duplicar uma associação.
    for (unsigned int i = 0; i < image.size(); i++)
        if (image.at(i) == img)
            insert = false;
    // Fazemos a nova associação.
    if (insert)
        image.push_back(img);
}

void PhotoTri::putPoint(Point *pt)
{
    bool insert = true;
    // Eliminamos primeiro a possibilidade duplicar uma associação.
    for (unsigned int i = 0; i < point.size(); i++)
        if (point.at(i) == pt)
            insert = false;
    // Fazemos a nova associação.
    if (insert)
        point.push_back(pt);
}

}
}
}
}
