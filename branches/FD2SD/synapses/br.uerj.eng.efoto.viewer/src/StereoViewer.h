#ifndef BR_UERJ_ENG_EFOTO_VIEWER_STEREOVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_STEREOVIEWER_H

#include "IStereoViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class StereoViewer
{
public:
    StereoViewer();
    ~StereoViewer();
    void startOpenLeftImageDialog();
    void startOpenRightImageDialog();
    void startSaveLeftImageDialog();
    void startSaveRightImageDialog();
    void markMode();
    void moveMode();
    void zoomInMode();
    void zoomOutMode();
    void hideOpen(bool status);
    void hideSave(bool status);
    void hideMark(bool status);
    void hideDetail(bool status);
    void hideOverView(bool status);
};

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_STEREOVIEWER_H
