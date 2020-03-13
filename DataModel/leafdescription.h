#ifndef LEAFDESCRIPTION_H
#define LEAFDESCRIPTION_H

struct LeafDescription
{
   int branchCount;
   int branchDepth;
   int texOctave;
   float texPersist;
   float texFreq;
   float shapeFq1;
   float shapeFq2;
   float shapeMin;
   float shapeMax;
   float thickness;
   float length;
   float lengthFct;
   float tilt;

   LeafDescription()
    : branchCount(6)
    , branchDepth(2)
    , texOctave(3)
    , texPersist(0.7)
    , texFreq(6.0)
    , shapeFq1(2.0)
    , shapeFq2(1.0)
    , shapeMin(0.0)
    , shapeMax(0.3)
    , thickness(0.02)
    , length(0.13)
    , lengthFct(0.9)
    , tilt(0.5)
   {}
};


#endif // LEAFDESCRIPTION_H
