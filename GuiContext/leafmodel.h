#ifndef LEAFMODEL_H
#define LEAFMODEL_H

#include <QObject>
#include "ProceduralScene/leafrenderer.h"

class LeafModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int branchdepth READ branchdepth WRITE setBranchdepth NOTIFY branchdepthChanged)
    Q_PROPERTY(int branchcount READ branchcount WRITE setBranchcount NOTIFY branchcountChanged)
    Q_PROPERTY(qreal branchtilt READ branchtilt WRITE setBranchtilt NOTIFY branchtiltChanged)
    Q_PROPERTY(qreal branchlength READ branchlength WRITE setBranchlength NOTIFY branchlengthChanged)
    Q_PROPERTY(qreal branchlengthfct READ branchlengthfct WRITE setBranchlengthfct NOTIFY branchlengthfctChanged)
    Q_PROPERTY(qreal branchthickness READ branchthickness WRITE setBranchthickness NOTIFY branchthicknessChanged)

    Q_PROPERTY(int texoctave READ texoctave WRITE setTexoctave NOTIFY texoctaveChanged)
    Q_PROPERTY(qreal texfreq READ texfreq WRITE setTexfreq NOTIFY texfreqChanged)
    Q_PROPERTY(qreal texpersist READ texpersist WRITE setTexpersist NOTIFY texpersistChanged)

    Q_PROPERTY(qreal shapefq1 READ shapefq1 WRITE setShapefq1 NOTIFY shapefq1Changed)
    Q_PROPERTY(qreal shapefq2 READ shapefq2 WRITE setShapefq2 NOTIFY shapefq2Changed)
    Q_PROPERTY(qreal shapemin READ shapemin WRITE setShapemin NOTIFY shapeminChanged)
    Q_PROPERTY(qreal shapemax READ shapemax WRITE setShapemax NOTIFY shapemaxChanged)


    Q_PROPERTY(int dft_branchdepth READ branchdepth WRITE setBranchdepth NOTIFY dft_branchdepthChanged)
    Q_PROPERTY(int dft_branchcount READ branchcount WRITE setBranchcount NOTIFY dft_branchcountChanged)
    Q_PROPERTY(qreal dft_branchtilt READ branchtilt WRITE setBranchtilt NOTIFY dft_branchtiltChanged)
    Q_PROPERTY(qreal dft_branchlength READ branchlength WRITE setBranchlength NOTIFY dft_branchlengthChanged)
    Q_PROPERTY(qreal dft_branchlengthfct READ branchlengthfct WRITE setBranchlengthfct NOTIFY dft_branchlengthfctChanged)
    Q_PROPERTY(qreal dft_branchthickness READ branchthickness WRITE setBranchthickness NOTIFY dft_branchthicknessChanged)

    Q_PROPERTY(int dft_texoctave READ texoctave WRITE setTexoctave NOTIFY dft_texoctaveChanged)
    Q_PROPERTY(qreal dft_texfreq READ texfreq WRITE setTexfreq NOTIFY dft_texfreqChanged)
    Q_PROPERTY(qreal dft_texpersist READ texpersist WRITE setTexpersist NOTIFY dft_texpersistChanged)

    Q_PROPERTY(qreal dft_shapefq1 READ shapefq1 WRITE setShapefq1 NOTIFY dft_shapefq1Changed)
    Q_PROPERTY(qreal dft_shapefq2 READ shapefq2 WRITE setShapefq2 NOTIFY dft_shapefq2Changed)
    Q_PROPERTY(qreal dft_shapemin READ shapemin WRITE setShapemin NOTIFY dft_shapeminChanged)
    Q_PROPERTY(qreal dft_shapemax READ shapemax WRITE setShapemax NOTIFY dft_shapemaxChanged)


    Q_PROPERTY(bool editing READ editing WRITE setEditing NOTIFY editingChanged)


public:
    LeafModel();

    static LeafModel* s_instance;

    int branchdepth() {return m_editDesc.branchDepth;}
    void setBranchdepth(int tf) {m_editDesc.branchDepth=tf; makeDirty();}

    int branchcount() {return m_editDesc.branchCount;}
    void setBranchcount(int tf) {m_editDesc.branchCount=tf; makeDirty();}

    qreal branchtilt() {return m_editDesc.tilt;}
    void setBranchtilt(qreal tf) {m_editDesc.tilt=tf; makeDirty();}

    qreal branchlength() {return m_editDesc.length;}
    void setBranchlength(qreal tf) {m_editDesc.length=tf; makeDirty();}

    qreal branchlengthfct() {return m_editDesc.lengthFct;}
    void setBranchlengthfct(qreal tf) {m_editDesc.lengthFct=tf; makeDirty();}

    qreal branchthickness() {return m_editDesc.thickness;}
    void setBranchthickness(qreal tf) {m_editDesc.thickness=tf; makeDirty();}



    int texoctave() {return m_editDesc.texOctave;}
    void setTexoctave(int tf) {m_editDesc.texOctave=tf; makeDirty();}

    qreal texfreq() {return m_editDesc.texFreq;}
    void setTexfreq(qreal tf) {m_editDesc.texFreq=tf; makeDirty();}

    qreal texpersist() {return m_editDesc.texPersist;}
    void setTexpersist(qreal tf) {m_editDesc.texPersist=tf; makeDirty();}



    qreal shapefq1() {return m_editDesc.shapeFq1;}
    void setShapefq1(qreal tf) {m_editDesc.shapeFq1=tf; makeDirty();}

    qreal shapefq2() {return m_editDesc.shapeFq2;}
    void setShapefq2(qreal tf) {m_editDesc.shapeFq2=tf; makeDirty();}

    qreal shapemin() {return m_editDesc.shapeMin;}
    void setShapemin(qreal tf) {m_editDesc.shapeMin=tf; makeDirty();}

    qreal shapemax() {return m_editDesc.shapeMax;}
    void setShapemax(qreal tf) {m_editDesc.shapeMax=tf; makeDirty();}



    bool editing() {return m_editing;}
    void setEditing(bool editing) {m_editing=editing;}



    const LeafDescription& getDesc() const {return m_editDesc;}

    bool hasChanged() const {return m_changed;}
    void makeReady() {m_changed=false;}
    void makeDirty() {m_changed=true;}

signals:

    void branchdepthChanged();
    void branchcountChanged();
    void branchtiltChanged();
    void branchlengthChanged();
    void branchlengthfctChanged();
    void branchthicknessChanged();

    void texoctaveChanged();
    void texfreqChanged();
    void texpersistChanged();

    void shapefq1Changed();
    void shapefq2Changed();
    void shapeminChanged();
    void shapemaxChanged();


    void dft_branchdepthChanged();
    void dft_branchcountChanged();
    void dft_branchtiltChanged();
    void dft_branchlengthChanged();
    void dft_branchlengthfctChanged();
    void dft_branchthicknessChanged();

    void dft_texoctaveChanged();
    void dft_texfreqChanged();
    void dft_texpersistChanged();

    void dft_shapefq1Changed();
    void dft_shapefq2Changed();
    void dft_shapeminChanged();
    void dft_shapemaxChanged();


    void editingChanged();

protected:

    LeafDescription m_editDesc;
    bool m_changed;
    bool m_editing;
};

#endif // LEAFMODEL_H
