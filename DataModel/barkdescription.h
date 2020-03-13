#ifndef BARKDESCRIPTION_H
#define BARKDESCRIPTION_H

struct BarkDescription
{
   int m_octaves;
   float m_freq;
   float m_persist;
   float m_scaleX;
   float m_scaleY;



   BarkDescription()
    : m_octaves(2)
    , m_freq(4.0)
    , m_persist(0.7)
    , m_scaleX(1.0)
    , m_scaleY(0.8)
   {}
};

#endif // BARKDESCRIPTION_H
