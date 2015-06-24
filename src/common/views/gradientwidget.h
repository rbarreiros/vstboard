#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QPen>

namespace View {

    class GradientWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit GradientWidget(QWidget *parent = 0);

        QColor getSelectedColor() {return m_selected_color;}
        int getRed() {return m_selected_color.red();}
        int getGreen() {return m_selected_color.green();}
        int getBlue() {return m_selected_color.blue();}
        qreal getAlpha() {return alpha;}
        qreal getHue() {return hue;}
        qreal getSaturation() {return saturation;}
        qreal getValue() {return value;}

    signals:
        void colorSelected(const QColor &color);

    public slots:
        void setMainColor(const QColor &col);
        void setRed(int r);
        void setGreen(int g);
        void setBlue(int b);
        void setAlpha(int a);
        void setHue(int h);
        void setSaturation(int s);
        void setValue(int v);



    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void enterEvent(QEvent *event);
        void leaveEvent(QEvent *event);
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);

    private:
        qreal hue,saturation,value,alpha;
        QColor m_selected_color;    // Couleur s�lectionn� dans le d�grad�
        QImage m_gradient_image;    // Image contenant le degrad� � afficher
        QPoint m_cursor_position;   // Position du point de s�lection
        bool m_left_button_pressed; // Bouton gauche de la souris press� ou non

        void updateGradientImage();
        void cursorMoved();
        void updateCursorPosition();
    };
}
#endif // GRADIENTWIDGET_H
