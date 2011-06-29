/**************************************************************************
#    Copyright 2010-2011 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#include "heap.h"
#ifndef QT_NO_DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "gradientwidget.h"

using namespace View;

const int default_diameter = 5;

GradientWidget::GradientWidget(QWidget *parent) :
        QWidget(parent),
        m_main_color(Qt::red),
        m_cursor_diameter(default_diameter),
        m_cursor_position(rect().center()),
        m_left_button_pressed(false)
{
    setMainColor(Qt::red);
    updateGradientImage();
}

GradientWidget::~GradientWidget(){}

QColor GradientWidget::selectedColor() const
{
    return m_selected_color;
}

void GradientWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::CrossCursor);
}

void GradientWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    unsetCursor();
}

void GradientWidget::mouseMoveEvent(QMouseEvent *event)
{
     if (!m_left_button_pressed) {
        event->ignore();
        return;
    }

    m_cursor_position.setX(event->pos().x());
    if (m_cursor_position.x() >= rect().width())
        m_cursor_position.setX(rect().width() - 1);
    if (m_cursor_position.x() < 0)
        m_cursor_position.setX(0);

    m_cursor_position.setY(event->pos().y());
    if (m_cursor_position.y() >= rect().height())
        m_cursor_position.setY(rect().height() - 1);
    if (m_cursor_position.y() < 0)
        m_cursor_position.setY(0);

    updateSelectedColor();
    emit colorSelected(m_selected_color);
    event->accept();
}

void GradientWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    m_cursor_position = event->pos();
    updateSelectedColor();
    emit colorSelected(m_selected_color);
    m_left_button_pressed = true;
    event->accept();
}

void GradientWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    if (rect().contains(event->pos())) {
        m_cursor_position = event->pos();
        updateSelectedColor();
        emit colorSelected(m_selected_color);
    }

    m_left_button_pressed = false;
    event->accept();
}

void GradientWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.drawImage(rect(), m_gradient_image);

    m_cursor_pen.setColor( (m_cursor_position.y() > (height() / 3)) ? Qt::white : Qt::black);
    painter.setPen(m_cursor_pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(m_cursor_position, m_cursor_diameter, m_cursor_diameter);
}

void GradientWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_cursor_position = rect().center();
    updateGradientImage();
    update();
}

void GradientWidget::setMainColor(const QColor &col)
{
    qreal h,s,v,a;
    col.getHsvF(&h,&s,&v,&a);
    m_cursor_position.setX( s*(width()-1) );
    m_cursor_position.setY( (1.0f-v)*(height()-1) );
    m_main_color = col;
    updateGradientImage();
}

void GradientWidget::setRed(int r)
{
    m_main_color.setRed(r);
    setMainColor(m_main_color);
}

void GradientWidget::setGreen(int g){
    m_main_color.setGreen(g);
    setMainColor(m_main_color);
}
void GradientWidget::setBlue(int b){
    m_main_color.setBlue(b);
    setMainColor(m_main_color);
}
void GradientWidget::setAlpha(int a){
    m_main_color.setAlpha(a);
    updateGradientImage();
}
void GradientWidget::setHue(int val){
    int h, s, v, a;
    m_main_color.getHsv(&h,&s,&v,&a);
    m_main_color.setHsv(val,s,v,a);
    updateGradientImage();
}

void GradientWidget::setCursorPen(const QPen &pen)
{
    m_cursor_pen = pen;
    update();
}

void GradientWidget::setCursorDiameter(const int diameter)
{
    m_cursor_diameter = diameter;
    update();
}

void GradientWidget::updateGradientImage()
{
    if (m_gradient_image.rect() != rect())
        m_gradient_image = QImage(size(), QImage::Format_ARGB32);

    float hue = m_main_color.hueF();
    if(hue<0.0f) hue=0.0;

    float alpha = m_main_color.alphaF();

    for (int s = 0; s < width(); ++s) {
        for (int v = 0; v < height(); ++v) {
            QColor color = QColor::fromHsvF(
                    hue,
                    (float)s / (width() - 1),
                    1.0f - ((float)v / (height() - 1)),
                    alpha);
            m_gradient_image.setPixel(s, v, color.rgba());
        }
    }
    updateSelectedColor();
}

void GradientWidget::updateSelectedColor()
{
    qreal h,s,v,a;
    m_main_color.getHsvF(&h,&s,&v,&a);
    s=(float)m_cursor_position.x()/(width()-1);
    v=1.0f - (float)m_cursor_position.y()/(height()-1);
    m_selected_color.setHsvF(h,s,v,a);
    update();
}
