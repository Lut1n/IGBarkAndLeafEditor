#include "leafmodel.h"

LeafModel* LeafModel::s_instance = nullptr;

LeafModel::LeafModel()
{
    s_instance = this;
    m_editing = false;
    makeDirty();
}
