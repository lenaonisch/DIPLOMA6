#include "StdAfx.h"
#include "MouseLabel.h"

MouseLabel::MouseLabel(QWidget *tab)
{
	x = -1;y = -1;
	dx = -1;dy = -1;
	rubberBand = NULL;
}

void MouseLabel::mousePressEvent(QMouseEvent * event)
{
	origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->hide();
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void MouseLabel::mouseMoveEvent(QMouseEvent * event)
{
	rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void MouseLabel::mouseReleaseEvent(QMouseEvent * event)
{
	QPoint cur = event->pos();
	if (cur.x() == origin.x() && cur.y() == origin.y())
	{
		x = -1;
		y = -1;
		dx = -1;
		dy = -1; return;
	}

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (cur.x() > this->size().width())
		cur.setX(this->size().width());
	if (cur.y() > this->size().height())
		cur.setY(this->size().height());

	if (origin.x() > cur.x())
	{
		x = cur.x();
		dx = origin.x() - x;
	}
	else
	{
		x = origin.x();
		dx = cur.x() - x;
	}

	if (origin.y() > cur.y())
	{
		y = cur.y();
		dy = origin.y() - y;
	}
	else
	{
		y = origin.y();
		dy = cur.y() - y;
	}
}
