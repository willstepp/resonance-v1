 #include <QtCore>
 #include <QtGui>
 
 class AeroButton : public QPushButton
 {
    Q_OBJECT
 
 public:
    AeroButton(QWidget * parent = 0);
    AeroButton(const QString & text, QWidget * parent = 0);
    AeroButton(const QIcon & icon, const QString & text, QWidget * parent = 0);
 
    virtual ~AeroButton();
 
    void setColor(QColor & color)         { m_color = color; }
    void setHighlight(QColor & highlight) { m_highlight = highlight; }
    void setShadow(QColor & shadow)       { m_shadow = shadow; }
 
    //Range: 0.0 [invisible] - 1.0 [opaque]
    void setOpacity(qreal opacity)        { m_opacity = opacity; }
 
    //Range: 0 [rectangle] - 99 [oval]
    void setRoundness(int roundness)      { m_roundness = roundness; }
 
 protected:
    void paintEvent(QPaintEvent * pe);
 
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
 
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
 
 private:
    QRect calculateIconPosition(QRect button_rect, QSize icon_size);
 
 private:
    bool m_hovered;
    bool m_pressed;
 
    QColor m_color;
    QColor m_highlight;
    QColor m_shadow;
 
    qreal m_opacity;
 
    int m_roundness;
 };