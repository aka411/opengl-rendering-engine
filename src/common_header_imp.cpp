//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../include/stb_image.h"
//#include "../include/stb_image_write.h"

/*The problem this file is used to solve is that both tingltf and 
my GPUTextureManager use stb, since stb is single header libary we are centralising the
implementation so as to avoid redefinitions*/


/*I have commented out stb headers here cause tiny gltf itself has stb libraries inside */

#define TINYGLTF_IMPLEMENTATION

//#define TINYGLTF_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/tiny_gltf.h"