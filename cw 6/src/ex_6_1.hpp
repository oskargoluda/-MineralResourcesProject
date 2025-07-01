#pragma once

#include "glew.h"
#include "Shader_Loader.h"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "Texture.h"

#include <vector>
#include <cmath>
#include "tiny_obj_loader.h"
    Core::Shader_Loader loader;
GLuint vao, vbo, program, tekstura;

glm::vec3 kamera;
glm::vec3 cel = glm::vec3(0.0f, 0.0f, 0.0f);
float aspekt = 1.3f;
int wybranaKostka = -1; // -1 oznacza brak wyboru




GLuint modelVao, modelVbo;
std::vector<float> modelVertices;
bool loadObjModel(const std::string& path, std::vector<float>& vertices);
GLuint klodawaVao, klodawaVbo;
std::vector<float> klodawaVertices;
GLuint teksturaKlodawa;


glm::vec3 kameraLegnica = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 kameraKlodawa = glm::vec3(-5.0f, 1.0f, 1.0f);
glm::vec3 celLegnica = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 celKlodawa = glm::vec3(-5.0f, 0.0f, 0.0f);


void init(GLFWwindow* win) {
    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    aspekt = w / float(h);

    glfwSetFramebufferSizeCallback(win, [](GLFWwindow*, int W, int H) {
        aspekt = W / float(H);
        glViewport(0, 0, W, H);
        });

    glEnable(GL_DEPTH_TEST);

    if (!loadObjModel("models/Legnica-Glogow.obj", modelVertices)) {
        std::cerr << "Nie udało się załadować modelu!" << std::endl;
        exit(1);
    }

    glGenVertexArrays(1, &modelVao);
    glGenBuffers(1, &modelVbo);

    glBindVertexArray(modelVao);
    glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(float), modelVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    program = loader.CreateProgram("shaders/shader_5_1.vert", "shaders/shader_5_1.frag");
    tekstura = Core::LoadTexture("textures/Legnica-Glogow.png");
    if (!loadObjModel("models/Klodawa.obj", klodawaVertices)) {
        std::cerr << "Nie udało się załadować modelu Kłodawa!" << std::endl;
        exit(1);
    }

    glGenVertexArrays(1, &klodawaVao);
    glGenBuffers(1, &klodawaVbo);

    glBindVertexArray(klodawaVao);
    glBindBuffer(GL_ARRAY_BUFFER, klodawaVbo);
    glBufferData(GL_ARRAY_BUFFER, klodawaVertices.size() * sizeof(float), klodawaVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    teksturaKlodawa = Core::LoadTexture("textures/Klodawa.png");
}

void renderScene(GLFWwindow* win) {
    float t = (float)glfwGetTime();
    //kamera.x = 10.0f * cos(t / 2.0f);
    //kamera.z = 10.0f * sin(t / 2.0f);
    //kamera.y = 7.0f;
    //kamera = glm::vec3(1.0f, 1.0f, 1.0f);
    if (wybranaKostka == 1) {
        kamera = kameraKlodawa;
        cel = celKlodawa;
    }
    else {
        kamera = kameraLegnica;
        cel = celLegnica;
    }
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto widok = glm::lookAt(kamera, cel, glm::vec3(0, 1, 0));
    auto projekt = glm::perspective(glm::radians(45.f), aspekt, 0.1f, 100.f);

    glUseProgram(program);

    GLuint transformLoc = glGetUniformLocation(program, "transformation");
    GLuint matrixLoc = glGetUniformLocation(program, "modelMatrix");
    GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
    GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
    GLuint shininessLoc = glGetUniformLocation(program, "shininess");
    GLuint ambientLoc = glGetUniformLocation(program, "ambientLight");
    GLuint numLightsLoc = glGetUniformLocation(program, "numLights");
    GLuint lightDirsLoc = glGetUniformLocation(program, "lightDirs");
    GLuint baseColorLoc = glGetUniformLocation(program, "baseColor");

    glUniform3fv(cameraPosLoc, 1, &kamera[0]);
    glUniform3f(lightPosLoc, 0.0f, 1.0f, -0.5f);
    glUniform1f(shininessLoc, 50.0f);
    glUniform3f(ambientLoc, 0.2f, 0.2f, 0.2f);

    glm::vec3 lightDirs[] = {
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0,-1, 0},
        {0, 0, 1}, {0, 0,-1}
    };
    glUniform1i(numLightsLoc, 6);
    glUniform3fv(lightDirsLoc, 6, &lightDirs[0][0]);

    glm::mat4 model = glm::mat4(1.0f);

    // Dodaj obrót wokół osi Y względem czasu
    model = glm::rotate(model, t / 4, glm::vec3(0, 1, 0));

    glm::mat4 mv = projekt * widok * model;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &model[0][0]);

    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    // ------------------ RENDER LEGNICA ---------------------
    glBindVertexArray(modelVao);
    glBindTexture(GL_TEXTURE_2D, tekstura);
    glDrawArrays(GL_TRIANGLES, 0, modelVertices.size() / 11);

    // ------------------ RENDER KLODAWA ---------------------
    glm::mat4 modelKlodawa = glm::mat4(1.0f);
    modelKlodawa = glm::translate(modelKlodawa, glm::vec3(-5.0f, 0.0f, 0.0f)); // przesuń Kłodawę w bok
    modelKlodawa = glm::rotate(modelKlodawa, t / 4, glm::vec3(0, 1, 0)); // opcjonalny obrót

    glm::mat4 mvKlodawa = projekt * widok * modelKlodawa;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvKlodawa[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &modelKlodawa[0][0]);

    glBindVertexArray(klodawaVao);
    glBindTexture(GL_TEXTURE_2D, teksturaKlodawa);
    glDrawArrays(GL_TRIANGLES, 0, klodawaVertices.size() / 11);


    glBindVertexArray(0);
    glUseProgram(0);
    glfwSwapBuffers(win);
}

void shutdown(GLFWwindow*) {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &tekstura);
    glDeleteVertexArrays(1, &klodawaVao);
    glDeleteBuffers(1, &klodawaVbo);
    glDeleteTextures(1, &teksturaKlodawa);
    loader.DeleteProgram(program);

}

void processInput(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) wybranaKostka = 0;
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) wybranaKostka = 1;
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) wybranaKostka = 2;
    if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) wybranaKostka = 3;
}

void renderLoop(GLFWwindow* win) {
    while (!glfwWindowShouldClose(win)) {
        processInput(win);
        renderScene(win);
        glfwPollEvents();
    }
}
bool loadObjModel(const std::string& path, std::vector<float>& vertices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << "TinyObjLoader Error: " << warn + err << std::endl;
        return false;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            glm::vec3 normal = index.normal_index >= 0 ? glm::vec3(
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]) : glm::vec3(0, 1, 0);

            glm::vec2 uv = index.texcoord_index >= 0 ? glm::vec2(
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]) : glm::vec2(0, 0);

            glm::vec3 color(1.0f, 1.0f, 1.0f); // opcjonalnie

            vertices.insert(vertices.end(), { pos.x, pos.y, pos.z });
            vertices.insert(vertices.end(), { color.r, color.g, color.b });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { uv.x, uv.y });
        }
    }

    return true;
}
