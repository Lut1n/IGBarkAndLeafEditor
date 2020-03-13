#include "displayoptions.h"

#include <QDebug>

DisplayOptions* DisplayOptions::s_instance = nullptr;

DisplayOptions::DisplayOptions()
    : QObject()
{
    s_instance = this;
    m_phong = false;
    m_environment = false;
    m_shadow = false;
    m_ssao = false;
    m_bloom = false;

    m_gBufferIndex = 0;
    m_gBuffer = "Default";
}
