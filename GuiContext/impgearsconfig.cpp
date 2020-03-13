#include "impgearsconfig.h"

ImpGearsConfig* ImpGearsConfig::s_instance = nullptr;

ImpGearsConfig::ImpGearsConfig()
    : m_transparencyEnabled(false)
    , m_animationEnabled(true)
    , m_shadowRes(0)
    , m_environmentRes(0)
    , m_shadowSamples(0)
    , m_ssaoSamples(0)
    , m_ambient(0.3)
    , m_power(200.0)
{
    s_instance = this;
}
