#ifndef BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H

#include <QSharedPointer>
#include <QImage>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

enum TargetChannel {LEFT_CHANNEL, RIGHT_CHANNEL};

class IStereoViewer
{
public:
    virtual void startOpenLeftImageDialog() = 0;
    virtual void startOpenRightImageDialog() = 0;
    virtual void startSaveLeftImageDialog() = 0;
    virtual void startSaveRightImageDialog() = 0;
    virtual void markMode() = 0;
    virtual void moveMode() = 0;
    virtual void zoomInMode() = 0;
    virtual void zoomOutMode() = 0;
    virtual void hideOpen(bool status) = 0;
    virtual void hideSave(bool status) = 0;
    virtual void hideMark(bool status) = 0;
    virtual void hideDetail(bool status) = 0;
    virtual void hideOverView(bool status) = 0;
};

typedef QSharedPointer<IStereoViewer> IStereoViewerPtr;

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H
