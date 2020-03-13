#include "treemodel.h"

TreeModel* TreeModel::s_instance = nullptr;

TreeModel::TreeModel()
{
    desc = TreeDescriptor::create();
    leafDesc = TreeDescriptor::create();
    imp::Vec3 col = desc->_leafColor;
    m_leafColor = QColor(col[0]*255,col[1]*255,col[2]*255);
    imp::Vec3 bcol = desc->_barkColor;
    m_barkColor = QColor(bcol[0]*255,bcol[1]*255,bcol[2]*255);
    m_changed = false;
    m_editing = false;

    s_instance = this;
}
