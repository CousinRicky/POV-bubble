// bubble.inc version 2.0
// Persistence of Vision Raytracer include file
// POV-Ray Object Collection

// Copyright (C) 2009 Charles Robertson.  Some rights reserved.
// Modifications copyright (C) 2025 Richard Callwood III.  Some rights reserved.
// This file is licensed under the terms of the GNU-LGPL
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Please
// visit https://www.gnu.org/licenses/lgpl-3.0.html for the text
// of the GNU Lesser General Public License version 3.

// Carbonated fluid bubble generator
// Created by Charles Robertson, 14 July 2009
// Version 2.0 modified by Richard Callwood III, 21 May 2025:
//  - Identifiers are brought into namespace compliance:
//      Macro mBubbleGenerator() is changed to Bubble_mGenerator().
//  - Macro Bubble_Generator() is added for untextured bubbles.
//  - The randomization is simplified, and a bias towards the +x+z quadrant is corrected.

// Optimised for POV-Ray 3.6 (works with 3.5 ... 3.8)

/**
Description: Creates columns of bubbles, representing CO2 inside a fluid.
Bubbles increase in size the higher they ascend, in line with a decrease in fluid pressure
Bubbles columns are placed randomly within a specified cylinder vector
Bubble start/end position can be modified
Bubble start/end radius can be modified
Bubble column quantity can be modified
Bubble texture can be modified
**/

/**
Parameters:

// REQUIRED
// a cylinder defines the boundaries of a the bubble area
cylinder{<sFluidX,sFluidY,sFluidZ>,<eFluidX,eFluidY,eFluidZ> fRadius}

// REQUIRED
// other parameters
fHeightRngE = the height at which the bubbles emanate, above the initial start height (sFluidY)
fBblHeightRngS = bubble start radius; the radius of bubbles at sFluidY
fBblHeightRngE = bubble end radius, the radius of bubbles at eFluidY
columnNm = amount of randomly placed bubble columns
bblTx = texture of bubble - required only in macro Bubble_mGenerator()
**/

/**
Example:
#include "bubble.inc"
#declare bblTexture = texture {finish {ambient 0.3 diffuse 0.1 reflection 0.25 specular 1 roughness 0.001} pigment {color rgbf <1,1,1,0.7>}}
#declare bubbleGenerator = Bubble_mGenerator(0,0.9,0,0,6.5,0,2.4,2.7,0.0005,0.0006,70,bblTexture)
**/




// Includes

#include "math.inc"
#include "transforms.inc"
#include "rand.inc"


// Macros

// Bubbles with a texture
#macro Bubble_mGenerator (sFluidX,sFluidY,sFluidZ,eFluidX,eFluidY,eFluidZ,fRadius,fHeightRngE,fBblHeightRngS,fBblHeightRngE,columnNm,bblTx)
  object
  { Bubble_Generator (sFluidX,sFluidY,sFluidZ,eFluidX,eFluidY,eFluidZ,fRadius,fHeightRngE,fBblHeightRngS,fBblHeightRngE,columnNm)
    texture { bblTx }
  }
#end

// Bubbles without a texture
#macro Bubble_Generator (sFluidX,sFluidY,sFluidZ,eFluidX,eFluidY,eFluidZ,fRadius,fHeightRngE,fBblHeightRngS,fBblHeightRngE,columnNm)
#local bubble_varInitHeightRangeStart = sFluidY;
#local bubble_varInitHeightRangeEnd = fHeightRngE;
#local bubble_varInitBubbleHeightRangeStart = fBblHeightRngS;
#local bubble_varInitBubbleHeightRangeEnd = fBblHeightRngE;
#local bubble_varInitHeight = RRand(bubble_varInitHeightRangeStart,bubble_varInitHeightRangeEnd,1);
#local bubble_varInitBubbleRange = RRand(bubble_varInitBubbleHeightRangeStart,bubble_varInitBubbleHeightRangeEnd,1);
#local bubble_varInitBubbleHeight = bubble_varInitBubbleRange;
#local bubble_varInitFluidHeight = eFluidY - sFluidY;
#local bubble_varInitBubbleNumber = (bubble_varInitFluidHeight/bubble_varInitBubbleHeight)/2;
#local bubble_varCounter = 1;

#local bubble_varColumnCount = 0;
#local bubble_varX = 0;
#local bubble_varZ = 0;
#local bubble_varXtest = bubble_varX+fRadius;
#local bubble_varZtest = bubble_varZ+fRadius;
#local bubble_varFluidX = eFluidX+fRadius;
#local bubble_varFluidZ = eFluidZ+fRadius;

union{

#while (bubble_varColumnCount<columnNm)
#while (bubble_varInitHeight<eFluidY)
sphere { <bubble_varX, bubble_varInitHeight, bubble_varZ>, bubble_varInitBubbleHeight }
#local bubble_varInitHeight = bubble_varInitHeight+(bubble_varInitBubbleHeight*bubble_varCounter)+(bubble_varInitBubbleHeight*2);
#local bubble_varInitBubbleHeight = bubble_varInitBubbleHeight + (bubble_varInitBubbleHeight*(bubble_varInitHeight/10));
#local bubble_varCounter = bubble_varCounter+1;
#end

#local bubble_varX = RRand (sFluidX - fRadius, sFluidX + fRadius, 1);
#local bubble_varZ = RRand (sFluidZ - fRadius, sFluidZ + fRadius, 1);

#if((bubble_varX+bubble_varInitBubbleHeight)>bubble_varFluidX)
#local bubble_varXAdj = (bubble_varX+bubble_varInitBubbleHeight)-bubble_varFluidX;
#local bubble_varX = bubble_varX-(bubble_varXAdj*2);
#end
#if((bubble_varZ+bubble_varInitBubbleHeight)>bubble_varFluidZ)
#local bubble_varZAdj = (bubble_varZ+bubble_varInitBubbleHeight)-bubble_varFluidZ;
#local bubble_varZ = bubble_varZ-(bubble_varZAdj*2);
#end

#local bubble_varInitHeight = RRand(bubble_varInitHeightRangeStart,bubble_varInitHeightRangeEnd,1);
#local bubble_varInitBubbleRange = RRand(bubble_varInitBubbleHeightRangeStart,bubble_varInitBubbleHeightRangeEnd,1);
#local bubble_varInitBubbleHeight = bubble_varInitBubbleRange;
#local bubble_varColumnCount = bubble_varColumnCount+1;
#local bubble_varCounter = 1;
#end

}

#end
