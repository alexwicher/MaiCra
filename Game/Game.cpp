#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../Cube/Cube.h"
#include "../TextureLoader/TextureLoader.h"
#include "../Camera/Camera.h"
#include "../gameSettings.h"
#include "../Control/Control.h"
#include "../MapLoader/MapLoader.h"
#include "../Resources/cubes/cubeTypes.h"
#include <iostream>
#include <unordered_map>
#include <stb_image.h>

int Game::startGame() {


    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();

    ShaderLoader shader = ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                       const_cast<char *>("../Shader/rtsShader.frag"));



    TextureLoader textureLoader = TextureLoader();
//    std::unordered_map<std::string, Cube *> flatRetardedMap;
//    std::array<unsigned int, 6> grassTextures = TextureLoader().loadCube(GRASS_BLOCK);
//    int x = 64, y = 2, z = 64;
//    for (int i = 0; i < x; ++i) {
//        for (int k = 0; k < y; ++k) {
//            for (int j = 0; j < z; ++j) {
//                flatRetardedMap[std::to_string(i) + "x" + std::to_string(k) + "y" + std::to_string(j) + "z"] = new Cube(
//                        i, k, j, grassTextures);
//            }
//        }
//    }
//    flatRetardedMap[std::to_string(x / 2 - 1) + "x" + std::to_string(y + 2) + "y" + std::to_string(z / 2 - 1) +
//                    "z"] = new Cube(x / 2, y + 2, z / 2, grassTextures);
//    MapLoader dumbMap = MapLoader(Camera(glm::vec3(0, 0, 0)),
//                                  ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
//                                               const_cast<char *>("../Shader/rtsShader.frag")),
//                                  flatRetardedMap);
//
//    dumbMap.initMap(glm::vec3(x / 2, y + 2, z / 2));

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Activate for lines
    //Main loop ...
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width=0, height=0, nrChannels=0;
    stbi_set_flip_vertically_on_load(true);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                // level
                 GL_RGBA8,         // Internal format
                 16, 16, 1, // width,height,depth
                 0,                // border?
                 GL_RGBA,          // format
                 GL_UNSIGNED_BYTE, // type
                 nullptr);
    unsigned char *data = stbi_load("../Resources/textures/Dirt.png", &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,    // GL_TEXTURE_2D_ARRAY
                        0,                    // mipmap level to load
                        0,                    // offset in an existing texture to load to
                        0,                    // offset in an existing texture to load to
                        0,                    // first array slice to load to (zoffset)
                        16,                    // width of data to be loaded
                        16,                    // height of data to be loaded
                        1,                    // number of array slices to load
                        GL_RGBA,                    // format of data to be loaded
                        GL_UNSIGNED_BYTE,                    // type of data to be loaded
                        data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    float s = 0.5, offx = 0, offy = 0, offz = 0;
    std::array<float, 24> squareMap = {
            -s + offx, s + offy, -s + offz, 0.0f, 1.0f,0,  //F-A -s s 0
            s + offx, s + offy, -s + offz, 1.0f, 1.0f,0,  //F-B s s 1
            s + offx, -s + offy, -s + offz, 1.0f, 0.0f,0,  //F-C s -s 2
            -s + offx, -s + offy, -s + offz, 0.0f, 0.0f,0  //F-D -s -s 3
    };

    unsigned int VBO, VAO, EBO;

    unsigned short indices[]  = {
            0,1,3,
            1,2,3
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, squareMap.size() * sizeof(float), &squareMap[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);

//     texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    Control control = Control();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Camera camera = Camera(glm::vec3(0,0,0));
    unsigned int textureArray = textureLoader.loadCubeTextures();

    shader.use();
    shader.setInt("texture1", 0);
    bool loop = true;
    unsigned int lastFrame = 0, deltaTime = 0;    // time between current frame and last frame
    while (loop) {
        control.handleCamera(&camera,&loop,window,deltaTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = camera.getProjection();
        shader.setMat4("projection", projection);

        // modelView transformation
        glm::mat4 view = camera.getView();
        shader.setMat4("modelView", view);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        unsigned int currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

//        dumbMap.handleControls(&loop, window, deltaTime);
//        dumbMap.loadMap();


        SDL_GL_SwapWindow(window);
    }
    return 0;

}

