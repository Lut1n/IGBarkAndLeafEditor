#include "barkmodel.h"

BarkModel* BarkModel::s_instance = nullptr;

BarkModel::BarkModel()
{
    s_instance = this;
    m_editing = false;
    makeDirty();
}
