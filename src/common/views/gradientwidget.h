#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPen>

namespace View {

    class GradientWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit GradientWidget(QWidget *parent = 0);
        ~GradientWidget();
        QColor  selectedColor() const;

    signals:
        void colorSelected(const QColor &color);

    public slots:
        void setMainColor(const QColor &col);
        void setCursorPen(const QPen &pen);
        void setCursorDiameter(const int diameter);
        void setRed(int r);
        void setGreen(int g);
        void setBlue(int b);
        void setAlpha(int a);
        void setHue(int h);

    protected:
        void    mouseMoveEvent(QMouseEvent *event);
        void    mousePressEvent(QMouseEvent *event);
        void    mouseReleaseEvent(QMouseEvent *event);
        void    enterEvent(QEvent *event);
        void    leaveEvent(QEvent *event);
        void    paintEvent(QPaintEvent *event);
        void    resizeEvent(QResizeEvent *event);

    private:
        QColor  m_main_color;        // Couleur principale du dégradé
        QColor  m_selected_color;    // Couleur sélectionné dans le dégradé
        QImage  m_gradient_image;    // Image contenant le degradé à afficher
        QPen    m_cursor_pen;        // QPen du point de sélection
        int     m_cursor_diameter;   // Diamètre du point de sélection
        QPoint  m_cursor_position;   // Position du point de sélection
        bool    m_left_button_pressed; // Bouton gauche de la souris pressé ou non

        void    updateGradientImage();
        void    updateSelectedColor();
    };
}
#endif // GRADIENTWIDGET_H
