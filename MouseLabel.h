#pragma once
#include "StdAfx.h"

#ifndef MOUSELABEL_H
#define MOUSELABEL_H


class MouseLabel : public QLabel
{
    Q_OBJECT
public:
    MouseLabel(QWidget *tab);

	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

	int x,y,dx,dy;
	QPoint origin;
	QRubberBand* rubberBand;
signals:
	void MousePressed();
	void MouseMoved();
	void MouseReleased();

public slots:

};

#endif // MOUSELABEL_H
