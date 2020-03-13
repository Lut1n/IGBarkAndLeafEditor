#ifndef BARKMODEL_H
#define BARKMODEL_H

#include <QObject>
#include "ProceduralScene/barkrenderer.h"

class BarkModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int octaves READ octaves WRITE setOctaves NOTIFY octavesChanged)
    Q_PROPERTY(qreal freq READ freq WRITE setFreq NOTIFY freqChanged)
    Q_PROPERTY(qreal persist READ persist WRITE setPersist NOTIFY persistChanged)
    Q_PROPERTY(qreal scaleX READ scaleX WRITE setScaleX NOTIFY scaleXChanged)
    Q_PROPERTY(qreal scaleY READ scaleY WRITE setScaleY NOTIFY scaleYChanged)
    // Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_PROPERTY(int dft_octaves READ octaves WRITE setOctaves NOTIFY dft_octavesChanged)
    Q_PROPERTY(qreal dft_freq READ freq WRITE setFreq NOTIFY dft_freqChanged)
    Q_PROPERTY(qreal dft_persist READ persist WRITE setPersist NOTIFY dft_persistChanged)
    Q_PROPERTY(qreal dft_scaleX READ scaleX WRITE setScaleX NOTIFY dft_scaleXChanged)
    Q_PROPERTY(qreal dft_scaleY READ scaleY WRITE setScaleY NOTIFY dft_scaleYChanged)

    Q_PROPERTY(bool editing READ editing WRITE setEditing NOTIFY editingChanged)

public:

    static BarkModel* s_instance;

    BarkModel();

    int octaves() { return m_editDesc.m_octaves; }
    float freq() { return m_editDesc.m_freq; }
    float persist() { return m_editDesc.m_persist; }
    float scaleX() { return m_editDesc.m_scaleX; }
    float scaleY() { return m_editDesc.m_scaleY; }
    // QColor color() { return m_color; }

    void setOctaves(int o) { m_editDesc.m_octaves = o; makeDirty(); emit octavesChanged(); }
    void setFreq(float f) { m_editDesc.m_freq = f; makeDirty(); emit freqChanged(); }
    void setPersist(float p) { m_editDesc.m_persist = p; makeDirty(); emit persistChanged(); }
    void setScaleX(float s) { m_editDesc.m_scaleX = s; makeDirty(); emit scaleXChanged(); }
    void setScaleY(float s) { m_editDesc.m_scaleY = s; makeDirty(); emit scaleYChanged(); }
    // void setColor(QColor c) { m_color = c;m_changed=true; emit colorChanged(); }

    // void changeUpdated(){m_changed=false;}
    // void notifyChange() {m_changed=true;}
    // bool hasChanged() {return m_changed;}

    bool editing() {return m_editing;}
    void setEditing(bool editing) {m_editing=editing;}



    const BarkDescription& getDesc() const {return m_editDesc;}

    bool hasChanged() const {return m_changed;}
    void makeReady() {m_changed=false;}
    void makeDirty() {m_changed=true;}

signals:
    void octavesChanged();
    void freqChanged();
    void persistChanged();
    void scaleXChanged();
    void scaleYChanged();

    void dft_octavesChanged();
    void dft_freqChanged();
    void dft_persistChanged();
    void dft_scaleXChanged();
    void dft_scaleYChanged();

    // void colorChanged();

    void editingChanged();

protected:

    // int m_octaves;
    // float m_freq;
    // float m_persist;
    // float m_scaleX;
    // float m_scaleY;
    // QColor m_color;
    BarkDescription m_editDesc;

    bool m_changed;
    bool m_editing;
};

#endif // BARKMODEL_H
