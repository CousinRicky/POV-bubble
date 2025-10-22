// bubble.inc version 4.0
// Persistence of Vision Raytracer include file
// POV-Ray Object Collection
//
// Carbonated fluid bubble generator
//
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

// Version 3.6 created by Charles Robertson, 14 July 2009
//
// Modifications by Richard Callwood III:
//
// Vers.  Date         Comments
// -----  ----         --------
// 2.0    2025-May-21  Identifiers are brought into namespace compliance:
//                       Macro mBubbleGenerator() is changed to Bubble_mGenerator().
//                     Macro Bubble_Generator() is added for untextured bubbles.
//                     The randomization is simplified, and a bias towards the +x+z quadrant is corrected.
// 2.0.1  2025-Aug-02  The first column of bubbles is randomized, rather than forced to the y-axis.
//                     The usual include file wrapper is added.
// 4.0    2025-Oct-22  The main macro is pretty-printed.
//                     Expansion rate of bubbles can be controlled.
//                     Short prefix 'bbl' is introduced.
//                     A version identifier is added.

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

Note: the above descriptions do not match the observed behavior of the macros;
please see the User Manual. --RC
**/

/**
Example:
#include "bubble.inc"
#declare bblTexture = texture {finish {ambient 0.3 diffuse 0.1 reflection 0.25 specular 1 roughness 0.001} pigment {color rgbf <1,1,1,0.7>}}
#declare bubbleGenerator = Bubble_mGenerator(0,0.9,0,0,6.5,0,2.4,2.7,0.0005,0.0006,70,bblTexture)
**/



#ifndef (Bubble_Inc_Temp) #declare Bubble_Inc_Temp = version;
#version max (3.5, min (3.8, version)); // Bracket the POV version.
#declare BUBBLE_VERSION = 4.0;

// Global parameters:

#ifndef (Bubble_Rate) #declare Bubble_Rate = 0.45; #end // a little more than linear pressure : cubic volume
#ifndef (bbl__Debug) #declare bbl__Debug = false; #end

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
#end // of macro Bubble_mGenerator()

// Bubbles without a texture
#macro Bubble_Generator (sFluidX,sFluidY,sFluidZ,eFluidX,eFluidY,eFluidZ,fRadius,fHeightRngE,fBblHeightRngS,fBblHeightRngE,columnNm)
  #local bbl_varInitHeightRangeStart = sFluidY;
  #local bbl_varInitHeightRangeEnd = fHeightRngE;
  #local bbl_varInitBubbleHeightRangeStart = fBblHeightRngS;
  #local bbl_varInitBubbleHeightRangeEnd = fBblHeightRngE;
  #local bbl_varInitHeight = RRand(bbl_varInitHeightRangeStart,bbl_varInitHeightRangeEnd,1);
  #local bbl_varInitBubbleRange = RRand(bbl_varInitBubbleHeightRangeStart,bbl_varInitBubbleHeightRangeEnd,1);
  #local bbl_varInitBubbleHeight = bbl_varInitBubbleRange;
  #local bbl_varInitFluidHeight = eFluidY - sFluidY;
  #local bbl_varInitBubbleNumber = (bbl_varInitFluidHeight/bbl_varInitBubbleHeight)/2;
  #local bbl_varCounter = 1;

  #local bbl_varColumnCount = 0;
  //#local bbl_varX = 0;
  //#local bbl_varZ = 0;
  #local bbl_varX = RRand (sFluidX - fRadius, sFluidX + fRadius, 1);
  #local bbl_varZ = RRand (sFluidZ - fRadius, sFluidZ + fRadius, 1);
  #local bbl_varXtest = bbl_varX+fRadius;
  #local bbl_varZtest = bbl_varZ+fRadius;
  #local bbl_varFluidX = eFluidX+fRadius;
  #local bbl_varFluidZ = eFluidZ+fRadius;

  union{

    #if (bbl__Debug) #debug concat ("Sizes ", vstr (2, <fBblHeightRngS, fBblHeightRngE>, ", ", 0, 4), ":\n") #end
    #while (bbl_varColumnCount<columnNm)
      #if (bbl__Debug) #debug concat ("Column ", str (bbl_varColumnCount, 0, 0), "\n") #end
      #local bbl_rFirst = bbl_varInitBubbleHeight;
      #while (bbl_varInitHeight<eFluidY)
        #local bbl_R = bbl_rFirst * pow (bbl_varInitBubbleHeight / bbl_rFirst, Bubble_Rate);
        //sphere { <bbl_varX, bbl_varInitHeight, bbl_varZ>, bbl_varInitBubbleHeight }
        sphere { <bbl_varX, bbl_varInitHeight, bbl_varZ>, bbl_R }
        #if (bbl__Debug)
          #debug concat
          ( "  <", vstr (3, <bbl_varX, bbl_varInitHeight, bbl_varZ>, ", ", 0, 3), ">, ",
            str (bbl_R, 0, 6), " // ", str (bbl_varInitBubbleHeight, 0, 6), "\n"
          )
        #end
        #local bbl_varInitHeight = bbl_varInitHeight+(bbl_varInitBubbleHeight*bbl_varCounter)+(bbl_varInitBubbleHeight*2);
        #local bbl_varInitBubbleHeight = bbl_varInitBubbleHeight + (bbl_varInitBubbleHeight*(bbl_varInitHeight/10));
        #local bbl_varCounter = bbl_varCounter+1;
      #end

      #local bbl_varX = RRand (sFluidX - fRadius, sFluidX + fRadius, 1);
      #local bbl_varZ = RRand (sFluidZ - fRadius, sFluidZ + fRadius, 1);

      #if((bbl_varX+bbl_varInitBubbleHeight)>bbl_varFluidX)
        #local bbl_varXAdj = (bbl_varX+bbl_varInitBubbleHeight)-bbl_varFluidX;
        #local bbl_varX = bbl_varX-(bbl_varXAdj*2);
      #end
      #if((bbl_varZ+bbl_varInitBubbleHeight)>bbl_varFluidZ)
        #local bbl_varZAdj = (bbl_varZ+bbl_varInitBubbleHeight)-bbl_varFluidZ;
        #local bbl_varZ = bbl_varZ-(bbl_varZAdj*2);
      #end

      #local bbl_varInitHeight = RRand(bbl_varInitHeightRangeStart,bbl_varInitHeightRangeEnd,1);
      #local bbl_varInitBubbleRange = RRand(bbl_varInitBubbleHeightRangeStart,bbl_varInitBubbleHeightRangeEnd,1);
      #local bbl_varInitBubbleHeight = bbl_varInitBubbleRange;
      #local bbl_varColumnCount = bbl_varColumnCount+1;
      #local bbl_varCounter = 1;
    #end

  }

#end // of macro Bubble_Generator()

#version Bubble_Inc_Temp;
#end
// end of bubble.inc
