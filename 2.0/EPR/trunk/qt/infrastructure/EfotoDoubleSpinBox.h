#ifndef EFOTODOUBLESPINBOX_H
#define EFOTODOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QEvent>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EfotoDoubleSpinBox: public QDoubleSpinBox
{
public:
	EfotoDoubleSpinBox(QWidget *parent=0);
	void keyPressEvent(QKeyEvent *event);

	bool eventFilter(QObject *, QEvent *);


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EFOTODOUBLESPINBOX_H
