/* bubble.pov version 2.0
 * Persistence of Vision Raytracer scene description file
 * POV-Ray Object Collection
 *
 * Copyright (C) 2025 Richard Callwood III.  Some rights reserved.
 * This file is licensed under the terms of the GNU-LGPL
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Please
 * visit https://www.gnu.org/licenses/lgpl-3.0.html for the text
 * of the GNU Lesser General Public License version 3.
 *
 * Vers  Date         Notes
 * ----  ----         -----
 * 2.0   21-May-2025  Created.
 */
// +W800 +H600 +A +R5
// +W160 +H120 +A +R5 Declare=Cam=2 +Obubble_thumbnail
#version 3.6;

#ifndef (Cam) #declare Cam = 1; #end

global_settings { assumed_gamma 1 max_trace_level 15 }

#include "bubble.inc"

#declare bblTexture = texture
{ finish
  { ambient 0.25 diffuse 1
    reflection 0.03
    specular 40 roughness 0.001
  }
  pigment { color rgbf <1, 0.9, 0.4, 0.85> }
}
Bubble_mGenerator (0, 3, 0, 0, 9, 0, 3, 5, 0.0005, 0.0006, 70, bblTexture)

light_source { <0, 100, 0>, rgb 1 }

camera
{ #if (Cam = 2)
    location <0, 12, -5>
    look_at <0, 6, 0>
    angle 60
  #else
    location <0, 8, -8>
    look_at <0, 6.5, 0>
    angle 60
  #end
}
