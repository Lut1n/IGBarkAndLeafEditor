#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QObject>
#include <QColor>

#include <DataModel/treedescriptor.h>

class TreeModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal baseRadius READ baseRadius WRITE setBaseRadius NOTIFY baseRadiusChanged)
    Q_PROPERTY(qreal baseHeight READ baseHeight WRITE setBaseHeight NOTIFY baseHeightChanged)
    Q_PROPERTY(int nodeCount READ nodeCount WRITE setNodeCount NOTIFY nodeCountChanged)
    Q_PROPERTY(int branchDiv READ branchDiv WRITE setBranchDiv NOTIFY branchDivChanged)
    Q_PROPERTY(qreal tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(qreal radiusFct READ radiusFct WRITE setRadiusFct NOTIFY radiusFctChanged)
    Q_PROPERTY(qreal heightFct READ heightFct WRITE setHeightFct NOTIFY heightFctChanged)
    Q_PROPERTY(qreal leafSize READ leafSize WRITE setLeafSize NOTIFY leafSizeChanged)
    Q_PROPERTY(qreal staggering READ staggering WRITE setStaggering NOTIFY staggeringChanged)
    Q_PROPERTY(int leafCount READ leafCount WRITE setLeafCount NOTIFY leafCountChanged)
    Q_PROPERTY(QColor leafColor READ leafColor WRITE setLeafColor NOTIFY leafColorChanged)
    Q_PROPERTY(QColor barkColor READ barkColor WRITE setBarkColor NOTIFY barkColorChanged)

    Q_PROPERTY(qreal dft_baseRadius READ baseRadius WRITE setBaseRadius NOTIFY dft_baseRadiusChanged)
    Q_PROPERTY(qreal dft_baseHeight READ baseHeight WRITE setBaseHeight NOTIFY dft_baseHeightChanged)
    Q_PROPERTY(int dft_nodeCount READ nodeCount WRITE setNodeCount NOTIFY dft_nodeCountChanged)
    Q_PROPERTY(int dft_branchDiv READ branchDiv WRITE setBranchDiv NOTIFY dft_branchDivChanged)
    Q_PROPERTY(qreal dft_tilt READ tilt WRITE setTilt NOTIFY dft_tiltChanged)
    Q_PROPERTY(qreal dft_radiusFct READ radiusFct WRITE setRadiusFct NOTIFY dft_radiusFctChanged)
    Q_PROPERTY(qreal dft_heightFct READ heightFct WRITE setHeightFct NOTIFY dft_heightFctChanged)
    Q_PROPERTY(qreal dft_leafSize READ leafSize WRITE setLeafSize NOTIFY dft_leafSizeChanged)
    Q_PROPERTY(qreal dft_staggering READ staggering WRITE setStaggering NOTIFY dft_staggeringChanged)
    Q_PROPERTY(int dft_leafCount READ leafCount WRITE setLeafCount NOTIFY dft_leafCountChanged)
    Q_PROPERTY(QColor dft_leafColor READ leafColor WRITE setLeafColor NOTIFY dft_leafColorChanged)
    Q_PROPERTY(QColor dft_barkColor READ barkColor WRITE setBarkColor NOTIFY dft_barkColorChanged)

    // Q_PROPERTY(bool changed READ hasChanged WRITE setChanged NOTIFY changedChanged)

    Q_PROPERTY(bool editing READ editing WRITE setEditing NOTIFY editingChanged)

public:


    static TreeModel* s_instance;

    TreeModel();

    float baseRadius() { return desc->_baseRadius; }
    float baseHeight() { return desc->_baseHeight; }
    int nodeCount() { return desc->_nodeCount; }
    int branchDiv() { return desc->_divisionCount; }
    float tilt() { return desc->_theta; }
    float radiusFct() { return desc->_radiusFactor; }
    float heightFct() { return desc->_heightFactor; }
    float leafSize() { return desc->_leafSize; }
    float staggering() {return desc->_staggering;}
    int leafCount() { return desc->_leafCount; }
    QColor leafColor() { return m_leafColor; }
    QColor barkColor() { return m_barkColor; }

    void setBaseRadius(float baseRadius) { desc->_baseRadius = baseRadius; makeDirty(); emit baseRadiusChanged();}
    void setBaseHeight(float baseHeight) { desc->_baseHeight = baseHeight; makeDirty(); emit baseHeightChanged();}
    void setNodeCount(int nodeCount) { desc->_nodeCount = nodeCount; makeDirty(); emit nodeCountChanged(); }
    void setBranchDiv(int branchdiv) { desc->_divisionCount = branchdiv; makeDirty(); emit branchDivChanged();}
    void setTilt(float t) { desc->_theta = t; makeDirty(); emit tiltChanged();}
    void setRadiusFct(float fct) { desc->_radiusFactor = fct; makeDirty(); emit radiusFctChanged();}
    void setHeightFct(float fct) { desc->_heightFactor = fct; makeDirty(); emit heightFctChanged();}
    void setLeafSize(float sz) { desc->_leafSize = sz; makeDirty(); emit leafSizeChanged();}
    void setStaggering(float sg) {desc->_staggering=sg; makeDirty(); emit staggeringChanged();}
    void setLeafCount(int c) { desc->_leafCount = c; makeDirty(); emit leafCountChanged();}
    void setLeafColor(QColor c) { m_leafColor = c; desc->_leafColor = Vec3(c.redF(), c.greenF(), c.blueF()); makeDirty(); emit leafColorChanged();}
    void setBarkColor(QColor c) { m_barkColor = c; desc->_barkColor = Vec3(c.redF(), c.greenF(), c.blueF()); makeDirty(); emit barkColorChanged();}

    TreeDescriptor::Ptr getDesc() {return desc;}
    TreeDescriptor::Ptr getLeafDesc() {return leafDesc;}


    bool editing() {return m_editing;}
    void setEditing(bool editing) {m_editing=editing;}

    bool hasChanged() const {return m_changed;}
    void makeReady() {m_changed=false;}
    void makeDirty() {m_changed=true;}

signals:
    void baseRadiusChanged();
    void baseHeightChanged();
    void nodeCountChanged();
    void branchDivChanged();
    void tiltChanged();
    void radiusFctChanged();
    void heightFctChanged();
    void leafSizeChanged();
    void staggeringChanged();
    void leafCountChanged();
    void leafColorChanged();
    void barkColorChanged();

    void dft_baseRadiusChanged();
    void dft_baseHeightChanged();
    void dft_nodeCountChanged();
    void dft_branchDivChanged();
    void dft_tiltChanged();
    void dft_radiusFctChanged();
    void dft_heightFctChanged();
    void dft_leafSizeChanged();
    void dft_staggeringChanged();
    void dft_leafCountChanged();
    void dft_leafColorChanged();
    void dft_barkColorChanged();

    void changedChanged();
    void editingChanged();

protected:

    QColor m_leafColor;
    QColor m_barkColor;
    TreeDescriptor::Ptr desc;
    TreeDescriptor::Ptr leafDesc;
    bool m_changed;
    bool m_editing;
};

#endif // TREEMODEL_H
