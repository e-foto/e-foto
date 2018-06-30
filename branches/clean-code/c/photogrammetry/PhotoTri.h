#ifndef PHOTOTRI_H
#define PHOTOTRI_H
/*Copyright 2002-2015 e-foto team (UERJ)
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

#include "EDom.h"
#include "Point.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class PhotoTri : public EDom
{
    int totalIterations{0};
    bool converged{false};
    double metricConvergency{0.0};
    double angularConvergency{0.0};
    double rmse{0.0};
    std::deque<int> imageKey{};
    std::deque<int> pointKey{};
    std::deque<Image*> image{};
    std::deque<Point*> point{};

public:
    PhotoTri();
    virtual ~PhotoTri(){}

    std::string xmlGetData();
    void xmlSetData(std::string xml);

    int getTotalIterations() const {return totalIterations;}
    bool getConverged() const {return converged;}
    double getMetricConvergency() const {return metricConvergency;}
    double getAngularConvergency() const {return angularConvergency;}
    double getRmse() const {return rmse;}

    std::deque<int> getImageKeys() const {return imageKey;}
    std::deque<int> getPointKeys() const {return pointKey;}
    std::deque<Image*> getImages() const {return image;}
    std::deque<Point*> getPoints() const {return point;}

    void putImage(Image* img);
    void putPoint(Point* pt);

    void clearImages(){image.clear();}
    void clearPoints(){point.clear();}
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
#endif
