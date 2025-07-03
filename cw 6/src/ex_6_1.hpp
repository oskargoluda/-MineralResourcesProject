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

enum ClippingMode {
    CLIPPING_OFF = 0,      
    CLIPPING_TOP_DOWN = 1,
    CLIPPING_LEFT_RIGHT = 2 
};

ClippingMode clippingMode = CLIPPING_OFF;
float clippingPosition = 0.0f;       
glm::vec4 clippingPlane = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const float CLIPPING_SPEED = 0.0005f;

const float ORBITAL_RADIUS = 1.2f;     // promien orbity billboardu
const float BILLBOARD_HEIGHT_OFFSET = 0.5f; // wysokosc billboardu nad obiektem

std::vector<float> createBillboardQuad() {
    return {
        // Pozycja (x,y,z), Kolor (r,g,b), Normala (nx,ny,nz), UV (u,v)
        // Trójkąt 1
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         // Trójkąt 2
         -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
          0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
    };
}

GLuint modelVao, modelVbo;
std::vector<float> modelVertices;
GLuint klodawaVao, klodawaVbo;
std::vector<float> klodawaVertices;
GLuint teksturaKlodawa;
GLuint melgiewVao, melgiewVbo, teksturaMelgiew;
std::vector<float> melgiewVertices;
GLuint adamowVao, adamowVbo, teksturaAdamow;
std::vector<float> adamowVertices;
GLuint glogowMiedzVao, glogowMiedzVbo;
std::vector<float> glogowMiedzVertices;
GLuint klodawaSolVao, klodawaSolVbo;
std::vector<float> klodawaSolVertices;
GLuint melgiewGazVao, melgiewGazVbo;
std::vector<float> melgiewGazVertices;
GLuint adamowWegleVao, adamowWegleVbo;
std::vector<float> adamowWegleVertices;
GLuint teksturaBilbordLegnica, teksturaBilbordKlodawa, teksturaBilbordMelgiew, teksturaBilbordAdamow;
GLuint bilbordLegnicaVao, bilbordLegnicaVbo;
GLuint bilbordKlodawaVao, bilbordKlodawaVbo;
GLuint bilbordMelgiewVao, bilbordMelgiewVbo;
GLuint bilbordAdamowVao, bilbordAdamowVbo;
std::vector<float> bilbordVertices;

glm::vec3 celLegnica = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 celKlodawa = glm::vec3(-5.0f, 0.0f, 0.0f);
glm::vec3 celMelgiew = glm::vec3(5.0f, 0.0f, 0.0f);
glm::vec3 celAdamow = glm::vec3(10.0f, 0.0f, 0.0f);

float cameraDistance = 2.0f;
float cameraAzimuth = 45.0f;
float cameraElevation = 30.0f;
const float MIN_DISTANCE = 1.0f;
const float MAX_DISTANCE = 2.5f;
const float MIN_ELEVATION = -80.0f;
const float MAX_ELEVATION = 80.0f;
const float CAMERA_ROTATION_SPEED = 0.02f;
const float CAMERA_ELEVATION_SPEED = 0.02f;
const float CAMERA_ZOOM_SPEED = 0.02f;

bool loadObjModel(const std::string& path, std::vector<float>& vertices);

glm::vec3 calculateCameraPosition(const glm::vec3& target) {
    float azimuthRad = glm::radians(cameraAzimuth);
    float elevationRad = glm::radians(cameraElevation);
    float x = target.x + cameraDistance * cos(elevationRad) * cos(azimuthRad);
    float y = target.y + cameraDistance * sin(elevationRad);
    float z = target.z + cameraDistance * cos(elevationRad) * sin(azimuthRad);
    return glm::vec3(x, y, z);
}

glm::vec3 calculateOrbitalBillboardPosition(const glm::vec3& objectCenter,
    const glm::vec3& cameraPos,
    float orbitRadius = ORBITAL_RADIUS,
    float heightOffset = BILLBOARD_HEIGHT_OFFSET) {
    glm::vec3 toCamera = cameraPos - objectCenter;
    toCamera.y = 0.0f;

    if (glm::length(toCamera) < 0.001f) {
        toCamera = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else {
        toCamera = glm::normalize(toCamera);
    }

    glm::vec3 billboardDir = -toCamera;
    glm::vec3 billboardPos = objectCenter + billboardDir * orbitRadius;
    billboardPos.y = objectCenter.y + heightOffset;

    return billboardPos;
}

glm::mat4 createBillboardMatrix(const glm::vec3& billboardPos, const glm::vec3& cameraPos, const glm::vec3& cameraUp = glm::vec3(0, 1, 0)) {
    glm::vec3 look = glm::normalize(cameraPos - billboardPos);
    glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
    glm::vec3 up = glm::cross(look, right);
    glm::mat4 billboardMatrix = glm::mat4(1.0f);
    billboardMatrix[0] = glm::vec4(right, 0.0f);    // os X 
    billboardMatrix[1] = glm::vec4(up, 0.0f);       // os Y
    billboardMatrix[2] = glm::vec4(look, 0.0f);     // os z
    billboardMatrix[3] = glm::vec4(billboardPos, 1.0f);
    return billboardMatrix;
}

void updateClippingPlane() {
    if (clippingMode == CLIPPING_TOP_DOWN) {
        // plaszczyzna pozioma -  gora/dol
        clippingPlane = glm::vec4(0.0f, 1.0f, 0.0f, -clippingPosition);
    }
    else if (clippingMode == CLIPPING_LEFT_RIGHT) {
        // plszczyzna pionowa - lewo/prawo
        float centerX = 0.0f; // domyslnie srodek
        switch (wybranaKostka) {
        case 0: centerX = celLegnica.x; break; 
        case 1: centerX = celKlodawa.x; break; 
        case 2: centerX = celMelgiew.x; break;  
        case 3: centerX = celAdamow.x; break;   
        default: centerX = 0.0f; break;         
        }
        // plaszczyzna przechodzi przez srodek aktualnego obiektu + offset
        float planeX = centerX + clippingPosition;
        clippingPlane = glm::vec4(1.0f, 0.0f, 0.0f, -planeX);
    }
}

void init(GLFWwindow* win) {
    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    aspekt = w / float(h);
    glfwSetFramebufferSizeCallback(win, [](GLFWwindow*, int W, int H) {
        aspekt = W / float(H);
        glViewport(0, 0, W, H);
        });
    glEnable(GL_DEPTH_TEST);

    // ---- Billboard Legnica-Glogow ----
    bilbordVertices = createBillboardQuad();
    glGenVertexArrays(1, &bilbordLegnicaVao);
    glGenBuffers(1, &bilbordLegnicaVbo);
    glBindVertexArray(bilbordLegnicaVao);
    glBindBuffer(GL_ARRAY_BUFFER, bilbordLegnicaVbo);
    glBufferData(GL_ARRAY_BUFFER, bilbordVertices.size() * sizeof(float), bilbordVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- Billboard Klodawa ----
    glGenVertexArrays(1, &bilbordKlodawaVao);
    glGenBuffers(1, &bilbordKlodawaVbo);
    glBindVertexArray(bilbordKlodawaVao);
    glBindBuffer(GL_ARRAY_BUFFER, bilbordKlodawaVbo);
    glBufferData(GL_ARRAY_BUFFER, bilbordVertices.size() * sizeof(float), bilbordVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- Billboard Melgiew ----
    glGenVertexArrays(1, &bilbordMelgiewVao);
    glGenBuffers(1, &bilbordMelgiewVbo);
    glBindVertexArray(bilbordMelgiewVao);
    glBindBuffer(GL_ARRAY_BUFFER, bilbordMelgiewVbo);
    glBufferData(GL_ARRAY_BUFFER, bilbordVertices.size() * sizeof(float), bilbordVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- Billboard Adamow ----
    glGenVertexArrays(1, &bilbordAdamowVao);
    glGenBuffers(1, &bilbordAdamowVbo);
    glBindVertexArray(bilbordAdamowVao);
    glBindBuffer(GL_ARRAY_BUFFER, bilbordAdamowVbo);
    glBufferData(GL_ARRAY_BUFFER, bilbordVertices.size() * sizeof(float), bilbordVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- LEGNICA-GLOGOW ----
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

    // ---- KLODAWA ----
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

    // ---- MELGIEW ----
    if (!loadObjModel("models/Melgiew.obj", melgiewVertices)) {
        std::cerr << "Nie udało się załadować modelu Melgiew!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &melgiewVao);
    glGenBuffers(1, &melgiewVbo);
    glBindVertexArray(melgiewVao);
    glBindBuffer(GL_ARRAY_BUFFER, melgiewVbo);
    glBufferData(GL_ARRAY_BUFFER, melgiewVertices.size() * sizeof(float), melgiewVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- ADAMOW ----
    if (!loadObjModel("models/Adamow.obj", adamowVertices)) {
        std::cerr << "Nie udało się załadować modelu Adamow!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &adamowVao);
    glGenBuffers(1, &adamowVbo);
    glBindVertexArray(adamowVao);
    glBindBuffer(GL_ARRAY_BUFFER, adamowVbo);
    glBufferData(GL_ARRAY_BUFFER, adamowVertices.size() * sizeof(float), adamowVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- LEGNICA-GLOGOW ZLOZE MIEDZI ----
    if (!loadObjModel("models/glogow_legnica_miedź.obj", glogowMiedzVertices)) {
        std::cerr << "Nie udało się załadować modelu złoża miedzi!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &glogowMiedzVao);
    glGenBuffers(1, &glogowMiedzVbo);
    glBindVertexArray(glogowMiedzVao);
    glBindBuffer(GL_ARRAY_BUFFER, glogowMiedzVbo);
    glBufferData(GL_ARRAY_BUFFER, glogowMiedzVertices.size() * sizeof(float), glogowMiedzVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- KLODAWA ZLOZE SOLI ----
    if (!loadObjModel("models/klodawa_zloze_soli.obj", klodawaSolVertices)) {
        std::cerr << "Nie udało się załadować modelu złoża soli!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &klodawaSolVao);
    glGenBuffers(1, &klodawaSolVbo);
    glBindVertexArray(klodawaSolVao);
    glBindBuffer(GL_ARRAY_BUFFER, klodawaSolVbo);
    glBufferData(GL_ARRAY_BUFFER, klodawaSolVertices.size() * sizeof(float), klodawaSolVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- MELGIEW ZLOZE GAZU ----
    if (!loadObjModel("models/mełgiew_obok_lublina_gaz_ziemny.obj", melgiewGazVertices)) {
        std::cerr << "Nie udało się załadować modelu złoża gazu!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &melgiewGazVao);
    glGenBuffers(1, &melgiewGazVbo);
    glBindVertexArray(melgiewGazVao);
    glBindBuffer(GL_ARRAY_BUFFER, melgiewGazVbo);
    glBufferData(GL_ARRAY_BUFFER, melgiewGazVertices.size() * sizeof(float), melgiewGazVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // ---- ADAMOW ZLOZE WEGLA ----
    if (!loadObjModel("models/adamow_wegle_brunatne.obj", adamowWegleVertices)) {
        std::cerr << "Nie udało się załadować modelu złoża węgla!" << std::endl;
        exit(1);
    }
    glGenVertexArrays(1, &adamowWegleVao);
    glGenBuffers(1, &adamowWegleVbo);
    glBindVertexArray(adamowWegleVao);
    glBindBuffer(GL_ARRAY_BUFFER, adamowWegleVbo);
    glBufferData(GL_ARRAY_BUFFER, adamowWegleVertices.size() * sizeof(float), adamowWegleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    program = loader.CreateProgram("shaders/shader_5_1.vert", "shaders/shader_5_1.frag");
    tekstura = Core::LoadTexture("textures/Legnica-Glogow2.png");
    teksturaKlodawa = Core::LoadTexture("textures/Klodawa2.png");
    teksturaMelgiew = Core::LoadTexture("textures/Melgiew2.png");
    teksturaAdamow = Core::LoadTexture("textures/Adamow2.png");
    teksturaBilbordLegnica = Core::LoadTexture("textures/Legnica-GlogowOpis.png");
    teksturaBilbordKlodawa = Core::LoadTexture("textures/KlodawaOpis.png");
    teksturaBilbordMelgiew = Core::LoadTexture("textures/MielgiewOpis.png");
    teksturaBilbordAdamow = Core::LoadTexture("textures/AdamowOpis.png");
}

void renderScene(GLFWwindow* win) {
    float t = (float)glfwGetTime();

    if (clippingMode != CLIPPING_OFF) {
        updateClippingPlane();
    }

    switch (wybranaKostka) {
    case 0:
        cel = celLegnica;
        break;
    case 1:
        cel = celKlodawa;
        break;
    case 2:
        cel = celMelgiew;
        break;
    case 3:
        cel = celAdamow;
        break;
    default:
        cel = celLegnica;
        break;
    }

    kamera = calculateCameraPosition(cel);
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
    GLuint useTextureLoc = glGetUniformLocation(program, "useTexture");
    GLuint clippingEnabledLoc = glGetUniformLocation(program, "clippingEnabled");
    GLuint clippingPlaneLoc = glGetUniformLocation(program, "clippingPlane");

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

    // przekazanie parametrow plaszczyzny do shadera
    bool clippingActive = (clippingMode != CLIPPING_OFF);
    glUniform1i(clippingEnabledLoc, clippingActive ? 1 : 0);
    if (clippingActive) {
        glUniform4fv(clippingPlaneLoc, 1, &clippingPlane[0]);
    }

    // ------------------ RENDER LEGNICA ---------------------
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
    glm::mat4 mv = projekt * widok * model;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &model[0][0]);
    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform1i(useTextureLoc, 1);
    glBindVertexArray(modelVao);
    glBindTexture(GL_TEXTURE_2D, tekstura);
    glDrawArrays(GL_TRIANGLES, 0, modelVertices.size() / 11);

    // ------------------ RENDER LEGNICA ZLOZE MIEDZI ---------------------
    glm::mat4 copperModel = glm::mat4(1.0f);
    copperModel = glm::translate(copperModel, glm::vec3(0.0f, -0.11f, 0.0f));
    copperModel = glm::scale(copperModel, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 copperMv = projekt * widok * copperModel;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &copperMv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &copperModel[0][0]);
    glUniform3f(baseColorLoc, 0.72f, 0.45f, 0.20f);
    glUniform1i(useTextureLoc, 0);
    glBindVertexArray(glogowMiedzVao);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawArrays(GL_TRIANGLES, 0, glogowMiedzVertices.size() / 11);

    // ------------------ RENDER KLODAWA ---------------------
    glm::mat4 modelKlodawa = glm::mat4(1.0f);
    modelKlodawa = glm::translate(modelKlodawa, glm::vec3(-5.0f, 0.0f, 0.0f));
    modelKlodawa = glm::scale(modelKlodawa, glm::vec3(0.8f, 0.8f, 0.8f));
    glm::mat4 mvKlodawa = projekt * widok * modelKlodawa;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvKlodawa[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &modelKlodawa[0][0]);
    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform1i(useTextureLoc, 1);
    glBindVertexArray(klodawaVao);
    glBindTexture(GL_TEXTURE_2D, teksturaKlodawa);
    glDrawArrays(GL_TRIANGLES, 0, klodawaVertices.size() / 11);

    // ------------------ RENDER KLODAWA ZLOZE SOLI ---------------------
    glm::mat4 saltModel = glm::mat4(1.0f);
    saltModel = glm::translate(saltModel, glm::vec3(-5.0f, -0.13f, 0.0f));
    saltModel = glm::scale(saltModel, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 saltMv = projekt * widok * saltModel;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &saltMv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &saltModel[0][0]);
    glUniform3f(baseColorLoc, 0.4f, 0.4f, 0.4f);
    glUniform1i(useTextureLoc, 0);
    glBindVertexArray(klodawaSolVao);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawArrays(GL_TRIANGLES, 0, klodawaSolVertices.size() / 11);

    // -------- RENDER MELGIEW --------
    glm::mat4 modelMelgiew = glm::mat4(1.0f);
    modelMelgiew = glm::translate(modelMelgiew, glm::vec3(5.0f, 0.0f, 0.0f));
    modelMelgiew = glm::scale(modelMelgiew, glm::vec3(0.8f, 0.8f, 0.8f));
    glm::mat4 mvMelgiew = projekt * widok * modelMelgiew;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvMelgiew[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &modelMelgiew[0][0]);
    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform1i(useTextureLoc, 1);
    glBindVertexArray(melgiewVao);
    glBindTexture(GL_TEXTURE_2D, teksturaMelgiew);
    glDrawArrays(GL_TRIANGLES, 0, melgiewVertices.size() / 11);

    // -------- RENDER MELGIEW ZLOZE GAZU --------
    glm::mat4 gasModel = glm::mat4(1.0f);
    gasModel = glm::translate(gasModel, glm::vec3(5.0f, -0.09f, 0.0f));
    gasModel = glm::scale(gasModel, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 gasMv = projekt * widok * gasModel;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &gasMv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &gasModel[0][0]);
    glUniform3f(baseColorLoc, 0.20f, 0.40f, 0.80f);
    glUniform1i(useTextureLoc, 0);
    glBindVertexArray(melgiewGazVao);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawArrays(GL_TRIANGLES, 0, melgiewGazVertices.size() / 11);

    // -------- RENDER ADAMOW --------
    glm::mat4 modelAdamow = glm::mat4(1.0f);
    modelAdamow = glm::translate(modelAdamow, glm::vec3(10.0f, 0.0f, 0.0f));
    modelAdamow = glm::scale(modelAdamow, glm::vec3(0.8f, 0.8f, 0.8f));
    glm::mat4 mvAdamow = projekt * widok * modelAdamow;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvAdamow[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &modelAdamow[0][0]);
    glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform1i(useTextureLoc, 1);
    glBindVertexArray(adamowVao);
    glBindTexture(GL_TEXTURE_2D, teksturaAdamow);
    glDrawArrays(GL_TRIANGLES, 0, adamowVertices.size() / 11);

    // -------- RENDER ADAMOW ZLOZE WEGLA --------
    glm::mat4 coalModel = glm::mat4(1.0f);
    coalModel = glm::translate(coalModel, glm::vec3(10.0f, -0.09f, 0.0f));
    coalModel = glm::scale(coalModel, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 coalMv = projekt * widok * coalModel;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &coalMv[0][0]);
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &coalModel[0][0]);
    glUniform3f(baseColorLoc, 0.25f, 0.15f, 0.05f);
    glUniform1i(useTextureLoc, 0);
    glBindVertexArray(adamowWegleVao);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawArrays(GL_TRIANGLES, 0, adamowWegleVertices.size() / 11);

    // ============ RENDER BILLBOARDY orbitralne ============
    glUniform1i(clippingEnabledLoc, 0);

    float billboardWidth = 1.0f;
    float billboardHeight = 0.6f;

    if (wybranaKostka == 0) { // Legnica-Glogow
        // orbitralna pozycja billboardu
        glm::vec3 billboardPos = calculateOrbitalBillboardPosition(celLegnica, kamera);
        // macierz billboardu zwrocona do kamery
        glm::mat4 billboardMatrix = createBillboardMatrix(billboardPos, kamera);
        billboardMatrix = glm::scale(billboardMatrix, glm::vec3(billboardWidth, billboardHeight, 1.0f));

        glm::mat4 mvBilbord = projekt * widok * billboardMatrix;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvBilbord[0][0]);
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &billboardMatrix[0][0]);
        glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform1i(useTextureLoc, 1);

        glBindVertexArray(bilbordLegnicaVao);
        glBindTexture(GL_TEXTURE_2D, teksturaBilbordLegnica);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else if (wybranaKostka == 1) { // Klodawa
        glm::vec3 billboardPos = calculateOrbitalBillboardPosition(celKlodawa, kamera);
        glm::mat4 billboardMatrix = createBillboardMatrix(billboardPos, kamera);
        billboardMatrix = glm::scale(billboardMatrix, glm::vec3(billboardWidth, billboardHeight, 1.0f));

        glm::mat4 mvBilbord = projekt * widok * billboardMatrix;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvBilbord[0][0]);
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &billboardMatrix[0][0]);
        glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform1i(useTextureLoc, 1);

        glBindVertexArray(bilbordKlodawaVao);
        glBindTexture(GL_TEXTURE_2D, teksturaBilbordKlodawa);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else if (wybranaKostka == 2) { // Melgiew
        glm::vec3 billboardPos = calculateOrbitalBillboardPosition(celMelgiew, kamera);
        glm::mat4 billboardMatrix = createBillboardMatrix(billboardPos, kamera);
        billboardMatrix = glm::scale(billboardMatrix, glm::vec3(billboardWidth, billboardHeight, 1.0f));

        glm::mat4 mvBilbord = projekt * widok * billboardMatrix;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvBilbord[0][0]);
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &billboardMatrix[0][0]);
        glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform1i(useTextureLoc, 1);

        glBindVertexArray(bilbordMelgiewVao);
        glBindTexture(GL_TEXTURE_2D, teksturaBilbordMelgiew);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else if (wybranaKostka == 3) { // Adamow
        glm::vec3 billboardPos = calculateOrbitalBillboardPosition(celAdamow, kamera);
        glm::mat4 billboardMatrix = createBillboardMatrix(billboardPos, kamera);
        billboardMatrix = glm::scale(billboardMatrix, glm::vec3(billboardWidth, billboardHeight, 1.0f));

        glm::mat4 mvBilbord = projekt * widok * billboardMatrix;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &mvBilbord[0][0]);
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &billboardMatrix[0][0]);
        glUniform3f(baseColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform1i(useTextureLoc, 1);

        glBindVertexArray(bilbordAdamowVao);
        glBindTexture(GL_TEXTURE_2D, teksturaBilbordAdamow);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

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
    glDeleteVertexArrays(1, &melgiewVao);
    glDeleteBuffers(1, &melgiewVbo);
    glDeleteTextures(1, &teksturaMelgiew);
    glDeleteVertexArrays(1, &adamowVao);
    glDeleteBuffers(1, &adamowVbo);
    glDeleteTextures(1, &teksturaAdamow);
    glDeleteVertexArrays(1, &glogowMiedzVao);
    glDeleteBuffers(1, &glogowMiedzVbo);
    glDeleteVertexArrays(1, &klodawaSolVao);
    glDeleteBuffers(1, &klodawaSolVbo);
    glDeleteVertexArrays(1, &melgiewGazVao);
    glDeleteBuffers(1, &melgiewGazVbo);
    glDeleteVertexArrays(1, &adamowWegleVao);
    glDeleteBuffers(1, &adamowWegleVbo);
    glDeleteVertexArrays(1, &bilbordLegnicaVao);
    glDeleteBuffers(1, &bilbordLegnicaVbo);
    glDeleteTextures(1, &teksturaBilbordLegnica);
    glDeleteVertexArrays(1, &bilbordKlodawaVao);
    glDeleteBuffers(1, &bilbordKlodawaVbo);
    glDeleteTextures(1, &teksturaBilbordKlodawa);
    glDeleteVertexArrays(1, &bilbordMelgiewVao);
    glDeleteBuffers(1, &bilbordMelgiewVbo);
    glDeleteTextures(1, &teksturaBilbordMelgiew);
    glDeleteVertexArrays(1, &bilbordAdamowVao);
    glDeleteBuffers(1, &bilbordAdamowVbo);
    glDeleteTextures(1, &teksturaBilbordAdamow);
    loader.DeleteProgram(program);
}

void processInput(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) wybranaKostka = 0;
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) wybranaKostka = 1;
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) wybranaKostka = 2;
    if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) wybranaKostka = 3;

    static bool cKeyPressed = false;

    // tryby: OFF -> TOP-DOWN -> LEFT-RIGHT -> OFF
    if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
        cKeyPressed = true;
        clippingMode = static_cast<ClippingMode>((clippingMode + 1) % 3);
        clippingPosition = 0.0f; // reset pozycji przy zmianie tryu

        switch (clippingMode) {
        case CLIPPING_OFF:
            std::cout << "Clipping: OFF" << std::endl;
            break;
        case CLIPPING_TOP_DOWN:
            std::cout << "Clipping: TOP-DOWN (vertical cross-section)" << std::endl;
            break;
        case CLIPPING_LEFT_RIGHT:
            std::cout << "Clipping: LEFT-RIGHT (horizontal cross-section)" << std::endl;
            break;
        }
    }

    if (glfwGetKey(win, GLFW_KEY_C) == GLFW_RELEASE) {
        cKeyPressed = false;
    }

    // kontrola plaszczyzny tnacej
    if (clippingMode != CLIPPING_OFF) {
        if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
            clippingPosition += CLIPPING_SPEED; 
        }
        if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
            clippingPosition -= CLIPPING_SPEED;
        }
    }

    glfwSetScrollCallback(win, [](GLFWwindow* window, double xoffset, double yoffset) {
        cameraDistance -= yoffset * CAMERA_ZOOM_SPEED;
        if (cameraDistance < MIN_DISTANCE) cameraDistance = MIN_DISTANCE;
        if (cameraDistance > MAX_DISTANCE) cameraDistance = MAX_DISTANCE;
        });

    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) cameraAzimuth -= CAMERA_ROTATION_SPEED;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) cameraAzimuth += CAMERA_ROTATION_SPEED;
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        cameraElevation += CAMERA_ELEVATION_SPEED;
        if (cameraElevation > MAX_ELEVATION) cameraElevation = MAX_ELEVATION;
    }
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        cameraElevation -= CAMERA_ELEVATION_SPEED;
        if (cameraElevation < MIN_ELEVATION) cameraElevation = MIN_ELEVATION;
    }
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraDistance -= CAMERA_ZOOM_SPEED;
        if (cameraDistance < MIN_DISTANCE) cameraDistance = MIN_DISTANCE;
    }
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
        cameraDistance += CAMERA_ZOOM_SPEED;
        if (cameraDistance > MAX_DISTANCE) cameraDistance = MAX_DISTANCE;
    }
    if (cameraAzimuth >= 360.0f) cameraAzimuth -= 360.0f;
    if (cameraAzimuth < 0.0f) cameraAzimuth += 360.0f;
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

            glm::vec3 color(1.0f, 1.0f, 1.0f);

            vertices.insert(vertices.end(), { pos.x, pos.y, pos.z });
            vertices.insert(vertices.end(), { color.r, color.g, color.b });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { uv.x, uv.y });
        }
    }
    return true;
}
