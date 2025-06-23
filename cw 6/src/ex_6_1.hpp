#pragma once

#include "glew.h"
#include "Shader_Loader.h"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "Texture.h"

#include <vector>
#include <cmath>

// === GLOBALNE ZMIENNE ===
Core::Shader_Loader loader;
GLuint vao, vbo, program, tekstura;

glm::vec3 kamera;
glm::vec3 cel = glm::vec3(0.0f, 0.0f, 0.0f);
float aspekt = 1.3f;
int wybranaKostka = -1; // -1 oznacza brak wyboru

// === DEKLARACJA FUNKCJI ===
std::vector<float> generateGrid(int size, float spacing);

// === GENERACJA SIATKI ===
std::vector<float> vert = generateGrid(35, 0.2f);

std::vector<float> generateGrid(int size, float spacing) {
    std::vector<float> vertices;
    for (int z = 0; z < size; ++z) {
        for (int x = 0; x < size; ++x) {
            float halfSize = size * spacing * 0.5f;
            float y = sin(x * 0.3f) * cos(z * 0.3f);
            glm::vec3 p0 = (glm::vec3(x, y, z) * spacing) - glm::vec3(halfSize, 0.0f, halfSize);

            glm::vec3 p1 = (glm::vec3(x + 1, 0.0f, z) * spacing) - glm::vec3(halfSize, 0.0f, halfSize);
            glm::vec3 p2 = (glm::vec3(x + 1, 0.0f, z + 1) * spacing) - glm::vec3(halfSize, 0.0f, halfSize);
            glm::vec3 p3 = (glm::vec3(x, 0.0f, z + 1) * spacing) - glm::vec3(halfSize, 0.0f, halfSize);


            glm::vec3 normal = glm::vec3(0, 1, 0);
            glm::vec3 color = glm::vec3(0.2f, 0.4f, 1.0f);
            glm::vec2 uv0(0, 0), uv1(1, 0), uv2(1, 1), uv3(0, 1);

            std::vector<glm::vec3> tri1 = { p0, p1, p2 };
            std::vector<glm::vec2> uvTri1 = { uv0, uv1, uv2 };
            std::vector<glm::vec3> tri2 = { p2, p3, p0 };
            std::vector<glm::vec2> uvTri2 = { uv2, uv3, uv0 };

            for (int i = 0; i < 3; ++i) {
                vertices.insert(vertices.end(), { tri1[i].x, tri1[i].y, tri1[i].z });
                vertices.insert(vertices.end(), { color.r, color.g, color.b });
                vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
                vertices.insert(vertices.end(), { uvTri1[i].x, uvTri1[i].y });
            }

            for (int i = 0; i < 3; ++i) {
                vertices.insert(vertices.end(), { tri2[i].x, tri2[i].y, tri2[i].z });
                vertices.insert(vertices.end(), { color.r, color.g, color.b });
                vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
                vertices.insert(vertices.end(), { uvTri2[i].x, uvTri2[i].y });
            }
        }
    }
    return vertices;
}

// === INICJALIZACJA ===
void init(GLFWwindow* win) {
    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    aspekt = w / float(h);

    glfwSetFramebufferSizeCallback(win, [](GLFWwindow*, int W, int H) {
        aspekt = W / float(H);
        glViewport(0, 0, W, H);
        });

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), vert.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    program = loader.CreateProgram("shaders/shader_5_1.vert", "shaders/shader_5_1.frag");
    //tekstura = Core::LoadTexture("../tekstura_1.png");
}

// === RENDER SCENY ===
void renderScene(GLFWwindow* win) {
    float t = (float)glfwGetTime();
    //kamera.x = 10.0f * cos(t / 2.0f);
    //kamera.z = 10.0f * sin(t / 2.0f);
    //kamera.y = 7.0f;
    kamera = glm::vec3(7.0f, 7.0f, 7.0f);
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
    model = glm::rotate(model, t / 2, glm::vec3(0, 1, 0));

    glm::mat4 mv = projekt * widok * model;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &model[0][0]);

    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);

    // Opcjonalnie: tekstura (odkomentuj jeśli shader obsługuje)
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, tekstura);
    // glUniform1i(glGetUniformLocation(program, "diffuseTexture"), 0);

    glDrawArrays(GL_TRIANGLES, 0, vert.size() / 11);

    glBindVertexArray(0);
    glUseProgram(0);
    glfwSwapBuffers(win);
}

// === ZAMYKANIE ===
void shutdown(GLFWwindow*) {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &tekstura);
    loader.DeleteProgram(program);
}

// === OBSŁUGA WEJŚCIA ===
void processInput(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) wybranaKostka = 0;
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) wybranaKostka = 1;
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) wybranaKostka = 2;
    if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) wybranaKostka = 3;
}

// === GŁÓWNA PĘTLA ===
void renderLoop(GLFWwindow* win) {
    while (!glfwWindowShouldClose(win)) {
        processInput(win);
        renderScene(win);
        glfwPollEvents();
    }
}
