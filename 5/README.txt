Possible Arguments:
-input barsscene.txt -size 300 300 -output edge.bmp
-input spheresolo.txt -size 300 300 -output testout.bmp
-input spheredua.txt -size 300 300 -output testout.bmp
-input cornell.txt -size 300 300 -output cornell.bmp


In order to select which filters are preferable, 
Set one of the following booleans in main.cpp to true:
simple : uses ambient occlusion only in the output.
edge: uses edge detection in the output. 
dof: uses depth of field in the output.

Additional variables that can be set in main.cpp:
focal_length, focal_num, botthreshbold, topthreshold are variables used in depth of field.
a DOF variable is calculated for each of the items (although actual distance would have been just fine), 
and the top and bottom thresholds are used to dictate which areas should be in focus. If a pixel's output falls within the thresholds,
it is set to be as sharp as possible. If not, blurness is applied to the pixel using the output of a guassian filter instead. 
(average blurring is stated in the report as a possible alternative.)


Additional variables that can be set in RayTracer.cpp:
ambient_occlusion_darkening is a vector which is our "penalty to pixels". This can be adjusted accordingly to increase or decrease the 
impact of ambient occlusion.

numberof_xyz_samples exponential in complexity. For example, a value of 3.0 will result in 27 rays being used to approximate the hemisphere.
5.0 will indicate that 125 rays should be used to approximate the hemisphere.

nooks_cranny the distance of T under which the ray will indicate it should contribute to ambient occlusion within the samples.