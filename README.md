# RayBox

Reference material for the implementations in this repository.

## `raybox.hpp`

Slab-style ray/AABB intersection.

Paper:
Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley.
"An efficient and robust ray-box intersection algorithm."
Journal of Graphics Tools 10, 54 (2005).
DOI: https://doi.org/10.1145/1198555.1198748

Related reading:
- Scratchapixel article:
  https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
- Scratchapixel example code:
  https://github.com/scratchapixel/scratchapixel-code/tree/main/minimal-ray-tracer-rendering-simple-shapes

## `refs/RayBoxIntersection.hpp`

Face-testing box/line intersection adapted to the local geometry types.

Source:
- 3dkingdoms article and snippet:
  http://www.3dkingdoms.com/weekly/weekly.php?a=3
