glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv

glslc sphere.frag -o sphere_frag.spv

glslc skybox.vert -o skybox_vert.spv

glslc skybox.frag -o skybox_frag.spv

glslc brdf_lut.vert -o brdf_lut_vert.spv

glslc brdf_lut.frag -o brdf_lut_frag.spv