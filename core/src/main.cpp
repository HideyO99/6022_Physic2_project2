#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3        (x,y,z)
#include <glm/vec4.hpp> // glm::vec4        (x,y,z,w)
#include <glm/mat4x4.hpp> // glm::mat4
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader/cShaderManager.h"
#include "VAOManager/cVAOManager.h"
#include "MeshObj/cMeshObj.h"
#include "Light/cLightManager.h"
#include "GUI/cGUI.h"
#include "Texture/cTextureManager.h"
#include "time.h"
#include "Physic/Physic.h"
#include "Animation/AnimationManager.h"
#include "FMOD/FmodManager.h"


#define MODEL_LIST_XML          "asset/model.xml"
#define VERTEX_SHADER_FILE      "src/shader/vertexShader.glsl"
#define FRAGMENT_SHADER_FILE    "src/shader/fragmentShader.glsl"
#define TEXTURE_PATH            "asset/texture"

glm::vec3 g_cameraEye = glm::vec3(0.0, 5.0, 0.0f);
glm::vec3 g_cameraTarget = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
float camAngle = 0.f;
bool bIsWalkAround = false;
bool firstMouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 1280.0f / 2.0;
float lastY = 800.0 / 2.0;
float fov = 45.0f;

double g_LastCall;
double g_LastCall5s;
double g_CurrentTime;

bool g_PlayAnimation = false;
unsigned int g_AnimationSeq = 0;

const int FRAMES_PER_SECOND = 30;
const double FRAME_RATE = (double)1 / FRAMES_PER_SECOND;

#define SEC_UPDATE 5

cLightManager* g_pTheLightManager = NULL;
static GLFWwindow* window = nullptr;
//cPhysicSystem g_physicSys;
cTextureManager* g_pTextureManager = NULL;
//cObject* g_player;

AnimationManager* g_pAnimationManager = NULL;

Physic* g_physic = nullptr;
cXML* xml = nullptr;
FModManager* fmodmanager = nullptr;
cMeshObj* g_meshBall = nullptr;
cVAOManager* gVAOManager = nullptr;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection);
void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection);
void light0Setup();
void light1Setup(cVAOManager* pVAOManager);
void light2Setup(cVAOManager* pVAOManager);
void light3Setup();
void light4Setup();
void checkBorder();

void createAnimation(cVAOManager* pVAOManager);

void updateByFrameRate();
bool FMOD_setup();

void createPhysXscene(cVAOManager* pVAOManager);

int main(void)
{

    bool result;
    std::cout << "starting..\n";
    //GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    //GLint mvp_location, vpos_location, vcol_location;
    GLuint shaderID = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWmonitor* pMainScreen = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(pMainScreen);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //window = glfwCreateWindow(mode->width, mode->height, "6028 Final", pMainScreen, NULL); //full screen
    window = glfwCreateWindow(1280, 800, "6022 Physic Project1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "created window" << std::endl;

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    GLFWcursor* crosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    

    glfwSetScrollCallback(window, scroll_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    srand((unsigned)time(NULL));

    /*initialize imgui*/
    cGUI* gui_ = new cGUI(&g_cameraEye,&g_cameraTarget);
    result = gui_->ImGUI_init(window);
    if (!result)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //create shader program
    cShaderManager* pShaderManager = new cShaderManager();
    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    vertexShader.fileName = VERTEX_SHADER_FILE;
    fragmentShader.fileName = FRAGMENT_SHADER_FILE;
    result = pShaderManager->createShaderProgram("Shader01", vertexShader, fragmentShader);
    if (!result)
    {
        std::cout << "error: Shader compilation fail" << std::endl;

        glfwDestroyWindow(window);

        delete pShaderManager;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "shader compilation OK" << std::endl;
    }

    pShaderManager->useShaderPRogram("Shader01");
    shaderID = pShaderManager->getIDfromName("Shader01");
    glUseProgram(shaderID);

    //todo lighting
    ::g_pTheLightManager = new cLightManager();


    
    ::g_pTheLightManager->loadLightUniformLocation(shaderID);
    for (size_t i = 0; i < MAX_LIGHT_SOURCE; i++)
    {
        gui_->pLight[i] = ::g_pTheLightManager->plight[i];
    }
    //load model
    cVAOManager* pVAOManager = new cVAOManager();

    result = pVAOManager->loadModelList(MODEL_LIST_XML, shaderID);
    if (!result)
    {
        std::cout << "cannot load model list" << std::endl;

        glfwDestroyWindow(window);

        delete pVAOManager;
        delete pShaderManager;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    ::g_cameraEye = pVAOManager->cameraEyeFromXML;

    //gui_->pMapInstanceNametoMeshObj = &pVAOManager->mapInstanceNametoMeshObj;
    gui_->pVecInstanceMeshObj = &pVAOManager->pVecInstanceMeshObj;

    //load texture
    ::g_pTextureManager = new cTextureManager();
    ::g_pTextureManager->setBasePath(TEXTURE_PATH);
    ::g_pTextureManager->create2DTextureFromBMP("Dungeons_2_Texture_01_A.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("lroc_color_poles_4k.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("glowing-fire-flame.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("glowing-fire-flame_bw.bmp");
    ::g_pTextureManager->create2DTextureFromBMP("photos_2018_7_4_fst_water-blue.bmp"); 
    ::g_pTextureManager->create2DTextureFromBMP("Beholder_Base_color.bmp");

    std::string load_texture_error = "";
    if (g_pTextureManager->createCubeTextureFromBMP("SpaceBox",
        "SpaceBox_right1_posX.bmp", /* positive X */
        "SpaceBox_left2_negX.bmp",  /* negative X */
        "SpaceBox_top3_posY.bmp",    /* positive Y */
        "SpaceBox_bottom4_negY.bmp",  /* negative Y */
        "SpaceBox_front5_posZ.bmp",  /* positive Z */
        "SpaceBox_back6_negZ.bmp", /* negative Z */
        true, load_texture_error))
    {
        std::cout << "Loaded the tropical sunny day cube map OK" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load the tropical sunny day cube map.->" << load_texture_error << std::endl;
    }
    //setup object

    result = pVAOManager->setDungeonTexture("floorA", "Dungeons_2_Texture_01_A.bmp");

    result = pVAOManager->setInstanceObjScale("terrain", 10);
    result = pVAOManager->setInstanceObjPosition("terrain", glm::vec4(0.f, -0.1f, 0.f, 0.f));
    result = pVAOManager->setTexture("terrain", "photos_2018_7_4_fst_water-blue.bmp", 0);
    result = pVAOManager->setSkyBoxFlag("skybox",true);

    //result = pVAOManager->setInstanceObjScale("bullet", 0.1);
    //result = pVAOManager->setInstanceObjRGB("bullet", glm::vec4(0.f, 0.f, 1.f, 1.f));
    //result = pVAOManager->setInstanceObjWireframe("Player", false);

    light0Setup(); // Dir light

    //Physic2 project 2 section
    g_physic = new Physic();
    g_physic->init();

    createPhysXscene(pVAOManager);


    //FMOD
    result = FMOD_setup();
    g_physic->collisionListener->fmod = fmodmanager;


    //Animation
    //::g_pAnimationManager = new AnimationManager();
    //createAnimation(pVAOManager);
    
    cTime::update();

    while (!glfwWindowShouldClose(window))
    {
        ::g_pTheLightManager->setLightToShader(shaderID);

        float ratio;
        int width, height;
        //glm::mat4x4 matModel;
        glm::mat4x4 matProjection;
        glm::mat4x4 matView;
        
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateByFrameRate();

        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(g_cameraEye - g_cameraTarget);
        glm::vec3 cameraRight = glm::normalize(glm::cross(g_upVector, cameraDirection));
        if (!bIsWalkAround)
        {

            matView = glm::lookAt(::g_cameraEye, ::g_cameraTarget, ::g_upVector);
        }
        else
        {
            matView = glm::lookAt(::g_cameraEye, ::g_cameraEye+::g_cameraFront, ::g_upVector);
        }
        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc, ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);

        matProjection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.0f);
        //matProjection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);
        //glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
        //glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));
        
        //glfwSetCursor(window, crosshairCursor);
        
        updateInstanceObj(pShaderManager, pVAOManager, matView, matProjection);



        gui_->ImGUICreateFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();

        //set window title
        //std::stringstream 
    }

    gui_->ImGUI_shutdown();
    fmodmanager->shutdown();

    glfwDestroyWindow(window);

    delete pVAOManager;
    delete pShaderManager;
    delete fmodmanager;
    delete xml;
    delete g_physic;
    //g_physic->~Physic();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection)
{
    glm::mat4x4 matModel;

    for (std::map<std::string, cMeshObj* >::iterator itCurrentMesh = pVAOManager->mapInstanceNametoMeshObj.begin();
        itCurrentMesh != pVAOManager->mapInstanceNametoMeshObj.end();
        itCurrentMesh++)
    {
        cMeshObj* pCurrentMeshObject = (itCurrentMesh->second);        // * is the iterator access thing

        //pCurrentMeshObject->textureRatios[0] -= 0.001f;
        //pCurrentMeshObject->textureRatios[1] += 0.001f;

        if (!pCurrentMeshObject->isVisible)
        {
            // Skip the rest of the loop
            continue;
        }

        if (pCurrentMeshObject->instanceName == "bullet")
        {
            //pCurrentMeshObject->position = ::g_cameraEye;

        }
        if (pCurrentMeshObject->isIslandModel)
        {
            pShaderManager->setShaderUniform1f("bIsIlandModel", (GLfloat)GL_TRUE);
        }
        if (pCurrentMeshObject->isSkybox)
        {
            pShaderManager->setShaderUniform1f("bIsSkyboxObject", (GLfloat)GL_TRUE);
            pCurrentMeshObject->position = ::g_cameraEye;
            pCurrentMeshObject->scale = glm::vec3(7500.f, 7500.f, 7500.f);
        }
        matModel = glm::mat4x4(1.0f);

        drawObj(pCurrentMeshObject, matModel, pShaderManager, pVAOManager, matView, matProjection);
        if (pCurrentMeshObject->isSkybox)
        {
            pShaderManager->setShaderUniform1f("bIsSkyboxObject", (GLfloat)GL_FALSE);
        }
        if (pCurrentMeshObject->isIslandModel)
        {
            pShaderManager->setShaderUniform1f("bIsIlandModel", (GLfloat)GL_FALSE);
        }
    }
}

void drawObj(cMeshObj* pCurrentMeshObject, glm::mat4x4 mat_PARENT_Model, cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection)
{
    // Don't draw any "back facing" triangles
    glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);
    // Turn on depth buffer test at draw time
    glEnable(GL_DEPTH_TEST);

    //matModel = glm::mat4x4(1.0f);  // identity matrix

#if 0   //flame effect
    pShaderManager->setShaderUniform1f("bIsFlameObject", (GLfloat)GL_TRUE);
    glDepthMask(GL_FALSE);
#endif


    pShaderManager->setShaderUniform1f("bUseDiscardTexture", (GLfloat)GL_FALSE);
    

    GLuint texture07_Number = g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[7]);
    GLuint texture07_Unit = 7;
    glActiveTexture(texture07_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture07_Number);
    pShaderManager->setShaderUniform1i("texture7", texture07_Unit);

    glm::mat4x4 matModel = mat_PARENT_Model;
    // Move the object 
    glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), pCurrentMeshObject->position);

    //std::cout << pCurrentMeshObject->instanceName << " position x = " << pCurrentMeshObject->position.x << " y = " << pCurrentMeshObject->position.y << " z = " << pCurrentMeshObject->position.z << std::endl;

    //rotate
    glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

    // Scale the object

    glm::mat4 matScale = glm::scale(glm::mat4(1.0f), pCurrentMeshObject->scale);

    matModel = matModel * matTranslation;

    matModel = matModel * matRoationX;
    matModel = matModel * matRoationY;
    matModel = matModel * matRoationZ;

    matModel = matModel * matScale;

    pShaderManager->setShaderUniformM4fv("mModel", matModel);
    pShaderManager->setShaderUniformM4fv("mView", matView);
    pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

    glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
    pShaderManager->setShaderUniformM4fv("mModelInverseTranspose", mModelInverseTransform);

    // Wireframe
    if (pCurrentMeshObject->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // GL_POINT, GL_LINE, GL_FILL
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pShaderManager->setShaderUniform4f("RGBA_Color",
        pCurrentMeshObject->color_RGBA.r,
        pCurrentMeshObject->color_RGBA.g,
        pCurrentMeshObject->color_RGBA.b,
        pCurrentMeshObject->color_RGBA.w);

    if (pCurrentMeshObject->bUse_RGBA_colour)
    {
        pShaderManager->setShaderUniform1f("bUseRGBA_Color", (GLfloat)GL_TRUE);
    }
    else
    {
        pShaderManager->setShaderUniform1f("bUseRGBA_Color", (GLfloat)GL_FALSE);
    }
    pShaderManager->setShaderUniform4f("specularColour",
        pCurrentMeshObject->specular_colour_and_power.r,
        pCurrentMeshObject->specular_colour_and_power.g,
        pCurrentMeshObject->specular_colour_and_power.b,
        pCurrentMeshObject->specular_colour_and_power.w);

    //uniform bool bDoNotLight;	
    if (pCurrentMeshObject->bDoNotLight)
    {
        pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_TRUE);
    }
    else
    {
        pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_FALSE);
    }

    //set texture0
    GLuint texture00_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[0]);
    GLuint texture00_Unit = 0;
    glActiveTexture(texture00_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture00_Number);
    pShaderManager->setShaderUniform1i("texture0", texture00_Unit);
    //set texture1
    GLuint texture01_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[1]);
    GLuint texture01_Unit = 1;
    glActiveTexture(texture01_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture01_Number);
    pShaderManager->setShaderUniform1i("texture1", texture01_Unit);
    //set texture2
    GLuint texture02_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[2]);
    GLuint texture02_Unit = 2;
    glActiveTexture(texture02_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture02_Number);
    pShaderManager->setShaderUniform1i("texture2", texture02_Unit);
    //set texture3
    GLuint texture03_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[3]);
    GLuint texture03_Unit = 3;
    glActiveTexture(texture03_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture03_Number);
    pShaderManager->setShaderUniform1i("texture3", texture03_Unit);
    //set texture4
    GLuint texture04_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[4]);
    GLuint texture04_Unit = 4;
    glActiveTexture(texture04_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture04_Number);
    pShaderManager->setShaderUniform1i("texture4", texture04_Unit);
    //set texture5
    GLuint texture05_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[5]);
    GLuint texture05_Unit = 5;
    glActiveTexture(texture05_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture05_Number);
    pShaderManager->setShaderUniform1i("texture5", texture05_Unit);
    //set texture6
    GLuint texture06_Number = ::g_pTextureManager->getTexttureID(pCurrentMeshObject->textures[6]);
    GLuint texture06_Unit = 6;
    glActiveTexture(texture06_Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture06_Number);
    pShaderManager->setShaderUniform1i("texture6", texture06_Unit);

    pShaderManager->setShaderUniform4f("texRatio_0_3",
        pCurrentMeshObject->textureRatios[0],
        pCurrentMeshObject->textureRatios[1],
        pCurrentMeshObject->textureRatios[2],
        pCurrentMeshObject->textureRatios[3]);

    //cube map texture
    GLuint cubeMapTextureNumber = g_pTextureManager->getTexttureID("SpaceBox");
    GLuint texture30Unit = 30;
    glActiveTexture(texture30Unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureNumber);
    pShaderManager->setShaderUniform1i("skyboxTexture", texture30Unit);

    cModelDrawInfo drawingInformation;
    if (pVAOManager->FindDrawInfo(pCurrentMeshObject->meshName, drawingInformation))
    {
        glBindVertexArray(drawingInformation.VAO_ID);

        glDrawElements(GL_TRIANGLES, drawingInformation.numberOfIndices, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

    }
    else
    {
        // Didn't find that model
        std::cout << "Error: didn't find model to draw." << std::endl;

    }

    for (std::vector<cMeshObj* >::iterator itCurrentMesh = pCurrentMeshObject->vecChildMesh.begin();
        itCurrentMesh != pCurrentMeshObject->vecChildMesh.end();
        itCurrentMesh++)
    {
        cMeshObj* pCurrentChildMeshObject = *itCurrentMesh;
        drawObj(pCurrentChildMeshObject, matModel, pShaderManager, pVAOManager, matView, matProjection);
    }

}

void light0Setup() //lamp
{
    //Directional light
    cDirLight* pDirLight = new cDirLight(*::g_pTheLightManager->plight[0]);
    *pDirLight->pDirection = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
    *pDirLight->pDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    *pDirLight->pSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
    *pDirLight->pTurnON = 1;

}

void light1Setup(cVAOManager* pVAOManager)
{
    //glm::vec4 atten = glm::vec4(0.7f, 0.1f, 0.2f, 1.0f);
    //glm::vec4 diffuse = glm::vec4(20.0f, 7.5f, 0.0f, 1.0f);
    //
    //::g_pTheLightManager->plight[1]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[1]->diffuse = diffuse;
    //::g_pTheLightManager->plight[1]->position = pVAOManager->getInstanceObjPosition("debug_light1");
    //::g_pTheLightManager->plight[1]->attenuation = atten;
    //::g_pTheLightManager->plight[1]->turnON = 1;

    //::g_pTheLightManager->plight[2]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[2]->diffuse = diffuse;
    //::g_pTheLightManager->plight[2]->position = pVAOManager->getInstanceObjPosition("debug_light2");
    //::g_pTheLightManager->plight[2]->attenuation = atten;
    //::g_pTheLightManager->plight[2]->turnON = 1;

    //::g_pTheLightManager->plight[3]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[3]->diffuse = diffuse;
    //::g_pTheLightManager->plight[3]->position = pVAOManager->getInstanceObjPosition("debug_light3");
    //::g_pTheLightManager->plight[3]->attenuation = atten;
    //::g_pTheLightManager->plight[3]->turnON = 1;

    //::g_pTheLightManager->plight[4]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[4]->diffuse = diffuse;
    //::g_pTheLightManager->plight[4]->position = pVAOManager->getInstanceObjPosition("debug_light4");
    //::g_pTheLightManager->plight[4]->attenuation = atten;
    //::g_pTheLightManager->plight[4]->turnON = 1;

    //::g_pTheLightManager->plight[5]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[5]->diffuse = diffuse;
    //::g_pTheLightManager->plight[5]->position = pVAOManager->getInstanceObjPosition("debug_light5");
    //::g_pTheLightManager->plight[5]->attenuation = atten;
    //::g_pTheLightManager->plight[5]->turnON = 1;

    //::g_pTheLightManager->plight[6]->type = cLight::LightType::LIGHT_POINT;
    //::g_pTheLightManager->plight[6]->diffuse = diffuse;
    //::g_pTheLightManager->plight[6]->position = pVAOManager->getInstanceObjPosition("debug_light6");
    //::g_pTheLightManager->plight[6]->attenuation = atten;
    //::g_pTheLightManager->plight[6]->turnON = 1;
}
void light2Setup(cVAOManager* pVAOManager)
{

    //::g_pTheLightManager->plight[7]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    //::g_pTheLightManager->plight[7]->diffuse = glm::vec4(2.0f, 0.f, 0.f, 1.0f);
    //::g_pTheLightManager->plight[7]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    //::g_pTheLightManager->plight[7]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[7]->direction = glm::vec4(-100.f, 0.f, 0.f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[7]->angle.x = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[7]->angle.y = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[7]->turnON = 1;

    //::g_pTheLightManager->plight[8]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    //::g_pTheLightManager->plight[8]->diffuse = glm::vec4(0.f, 1.f, 0.f, 1.0f);
    //::g_pTheLightManager->plight[8]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    //::g_pTheLightManager->plight[8]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[8]->direction = glm::vec4(100.f, -0.5f, 0.f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[8]->angle.x = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[8]->angle.y = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[8]->turnON = 1;

    //::g_pTheLightManager->plight[9]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    //::g_pTheLightManager->plight[9]->diffuse = glm::vec4(0.f, 0.f, 1.0f, 1.0f);
    //::g_pTheLightManager->plight[9]->attenuation = glm::vec4(1.19f, 0.1f, 0.2f, 1.0f);
    //::g_pTheLightManager->plight[9]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[9]->direction = glm::vec4(0.f, -0.5f, 100.f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[9]->angle.x = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[9]->angle.y = 5.0f;     // Degrees
    //::g_pTheLightManager->plight[9]->turnON = 1;
}

void light3Setup()
{

    //::g_pTheLightManager->plight[6]->position = glm::vec4(15.6f, 0.6f, 8.7f, 1.0f);
    //::g_pTheLightManager->plight[6]->diffuse = glm::vec4(0.8f, .50f, 0.4f, 1.0f);
    //::g_pTheLightManager->plight[6]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    //::g_pTheLightManager->plight[6]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[6]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[6]->angle.x = 10.0f;     // Degrees
    //::g_pTheLightManager->plight[6]->angle.y = 20.0f;     // Degrees
    //::g_pTheLightManager->plight[6]->turnON = 1;

    //::g_pTheLightManager->plight[7]->position = glm::vec4(15.0f, 0.6f, 9.5f, 1.0f);
    //::g_pTheLightManager->plight[7]->diffuse = glm::vec4(0.8f, .50f, 0.4f, 1.0f);
    //::g_pTheLightManager->plight[7]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    //::g_pTheLightManager->plight[7]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[7]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    //// inner and outer angles
    //::g_pTheLightManager->plight[7]->angle.x = 10.0f;     // Degrees
    //::g_pTheLightManager->plight[7]->angle.y = 20.0f;     // Degrees
    //::g_pTheLightManager->plight[7]->turnON = 1;

}

void light4Setup()
{
    //::g_pTheLightManager->plight[8]->position = glm::vec4(11.9f, 0.4f, 5.f, 1.0f);
    //::g_pTheLightManager->plight[8]->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.0f);
    //::g_pTheLightManager->plight[8]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    //::g_pTheLightManager->plight[8]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[8]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    //// inner and outer angles    
    //::g_pTheLightManager->plight[8]->angle.x = 10.0f;     // Degrees
    //::g_pTheLightManager->plight[8]->angle.y = 20.0f;     // Degrees
    //::g_pTheLightManager->plight[8]->turnON = 1;

    //::g_pTheLightManager->plight[9]->position = glm::vec4(11.2f, 0.6f, 5.7f, 1.0f);
    //::g_pTheLightManager->plight[9]->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.0f);
    //::g_pTheLightManager->plight[9]->attenuation = glm::vec4(0.19f, 0.003f, 0.072f, 1.0f);
    //::g_pTheLightManager->plight[9]->type = cLight::LightType::LIGHT_SPOT;
    //::g_pTheLightManager->plight[9]->direction = glm::vec4(0.07f, -0.5f, 1.0f, 1.0f);
    //// inner and outer angle
    //::g_pTheLightManager->plight[9]->angle.x = 10.0f;     // Degrees
    //::g_pTheLightManager->plight[9]->angle.y = 20.0f;     // Degrees
    //::g_pTheLightManager->plight[9]->turnON = 1;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    //move camera
    // AWSD AD-Left, Right
    //      WS-Forward, Back
    const float CAMERA_MOVE_SPEED = 5.f;
    const float MOVE_SPEED = 5.f;
    const float PI = 3.141f;
    if (key == GLFW_KEY_A)
    {
        glm::vec3 dir = glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1))));
        g_physic->UserForce(dir);
       
        //::g_cameraEye.x -= CAMERA_MOVE_SPEED;
        //::g_cameraEye += (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
        
    }
    if (key == GLFW_KEY_D)
    {
        glm::vec3 dir = -(glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)))));
        g_physic->UserForce(dir);
        //::g_cameraEye.x += CAMERA_MOVE_SPEED;
        //::g_cameraEye -= (glm::normalize(glm::cross(g_upVector, (::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED)));
    }
    if (key == GLFW_KEY_W)
    {
        glm::vec3 dir = ::g_cameraFront * glm::vec3(1, 0, 1);
        g_physic->UserForce(dir);
        //::g_cameraEye.z -= CAMERA_MOVE_SPEED;
        //::g_cameraEye += ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
    }
    if (key == GLFW_KEY_S)
    {
        glm::vec3 dir = -(::g_cameraFront * glm::vec3(1, 0, 1));
        g_physic->UserForce(dir);
        //::g_cameraEye.z += CAMERA_MOVE_SPEED;
        //::g_cameraEye -= ((::g_cameraFront * glm::vec3(1, 0, 1)) * CAMERA_MOVE_SPEED);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        glm::vec3 pos;
        pos.x = rand() % 32 - 16;
        pos.y = rand() % 20 + 5;
        pos.z = rand() % 32 - 16;
        float size = float(rand() % 4)+1;
        std::string instantName = "ball" + std::to_string(g_physic->m_ballList.size());
        gVAOManager->createOBJ("ball", instantName, pos, glm::vec3(size));
        gVAOManager->setInstanceObjRGB(instantName, glm::vec4(1, 0, 0, 1));
        gVAOManager->setInstanceObjLighting(instantName, true);
        g_physic->createBall(gVAOManager->findMeshObjAddr(instantName), pos, size);
        g_physic->setActive(g_physic->m_ballList.size()-1);
    }
    if (key == GLFW_KEY_UP)//&& action == GLFW_RELEASE)
    {
        fov -= 0.5;
    }
    if (key == GLFW_KEY_DOWN)//&& action == GLFW_RELEASE)
    {
        fov += 0.5;
    }

    if (key == GLFW_KEY_RIGHT )//&& action == GLFW_RELEASE)
    {
        camAngle += 0.1f;
        g_cameraEye.x = 90 * sin(camAngle);
        g_cameraEye.z = 90 * cos(camAngle);
        g_cameraFront.x = -sin(camAngle);
        g_cameraFront.z = -cos(camAngle);
    }

    if (key == GLFW_KEY_LEFT )//&& action == GLFW_RELEASE)
    {
        camAngle -= 0.1f;
        g_cameraEye.x = 90 * sin(camAngle);
        g_cameraEye.z = 90 * cos(camAngle);
        g_cameraFront.x = -sin(camAngle);
        g_cameraFront.z = -cos(camAngle);
    }

    if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
    {
        //g_physic->setActive(0);
    }

    if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
    {
       // g_physic->setActive(1);
    }

    if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
    {
       // g_physic->setActive(2);
    }

    if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
    {
       // g_physic->setActive(3);
    }

    if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
    {
      //  g_physic->setActive(4);
    }

    //checkBorder();
}
void checkBorder()
{
    //if (::g_cameraEye.x < -48)
    //{
    //    ::g_cameraEye.x = -48;
    //}
    //if (::g_cameraEye.x > 48)
    //{
    //    ::g_cameraEye.x = 48;
    //}
    //if (::g_cameraEye.z < -48)
    //{
    //    ::g_cameraEye.z = -48;
    //}
    //if (::g_cameraEye.z > 48)
    //{
    //    ::g_cameraEye.z = 48;
    //}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    //glutWarpPointer(x, y);
    //glfwSetMousePos(xposIn, yposIn);
    glfwSetCursorPos(window,xposIn, yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if (bIsWalkAround)
    {


        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.5f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        ::g_cameraFront = glm::normalize(front);
    }
    else
    {
        //::g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        //popup_menu();
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //std::map<std::string, cObject*>::iterator obj_it = g_physicSys.mapOBJ.find("bullet");
        //obj_it->second->position = ::g_cameraEye;
        //::g_physicSys.fire(::g_cameraFront);
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void updateByFrameRate()
{
    cTime::update();
    double deltaTime = cTime::getDeltaTime();
    g_CurrentTime += deltaTime;

    if (g_CurrentTime >= g_LastCall + FRAME_RATE)
    {
        double elapsedTime = g_CurrentTime - g_LastCall;
        g_LastCall = g_CurrentTime;

        g_physic->update(elapsedTime);
        fmodmanager->update();
        // 
        //std::map<std::string, cObject*>::iterator obj_it = g_physicSys.mapOBJ.find("Player");
        //obj_it->second->position = ::g_cameraEye;

        //cal player velocity
        //obj_it->second->velocity.x = (obj_it->second->position.x - obj_it->second->prevPosition.x)/ elapsedTime;
        //obj_it->second->velocity.z = (obj_it->second->position.z - obj_it->second->prevPosition.z) / elapsedTime;

        //obj_it->second->update();

        //g_pAnimationManager->AnimationUpdate(g_PlayAnimation, elapsedTime);
        //g_physicSys.updateSystem(elapsedTime);
    }
    //if (g_CurrentTime >= g_LastCall5s + SEC_UPDATE)
    //{
    //    double elapsedTime = g_CurrentTime - g_LastCall5s;
    //    g_LastCall5s = g_CurrentTime;

    //    g_physicSys.gameUpdate();
    //}
}

bool FMOD_setup()
{
    bool result;
    //init fmod
    fmodmanager = new FModManager();

    result = fmodmanager->Fmod_init(MAX_CHANNEL, FMOD_INIT_NORMAL);
    if (!result)
    {
        return -2;
    }

    result = fmodmanager->create_channel(MASTER_CH);
    result = fmodmanager->create_channel(FX1_CH);

    if (!result)
    {
        return -3;
    }

    result = fmodmanager->set_channel_parent(FX1_CH, MASTER_CH);

    if (!result)
    {
        return -4;
    }

    //read XML file
    xml = new cXML();
    xml->loadSoundFromXML();

    result = fmodmanager->set_channel_vol(MASTER_CH, 0.5f);

    result = fmodmanager->create_sound("fx1", xml->my_fx_path[0], FMOD_DEFAULT, true);



    return result;
}

void createAnimation(cVAOManager* pVAOManager)
{
    //setup animation properties
    cMeshObj* Enemy1 = pVAOManager->findMeshObjAddr("enemy1");
    Enemy1->Animation.tag = "PosLerpNoEasing";
    Enemy1->Animation.curSeq = 0;
    Enemy1->color_RGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
    Enemy1->Animation.Speed = 1.f;
    Enemy1->Animation.IsLooping = false;
    Enemy1->Animation.IsPlaying = false;
    Enemy1->Animation.AnimationTime = 0.f;
    g_pAnimationManager->animationOBJList.push_back(Enemy1);

    cMeshObj* Enemy2 = pVAOManager->findMeshObjAddr("enemy2");
    Enemy2->Animation.tag = "PosLerpEaseIn";
    Enemy2->Animation.curSeq = 0;
    Enemy2->color_RGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
    Enemy2->Animation.Speed = 1.f;
    Enemy2->Animation.IsLooping = false;
    Enemy2->Animation.IsPlaying = false;
    Enemy2->Animation.AnimationTime = 0.f;
    g_pAnimationManager->animationOBJList.push_back(Enemy2);

    cMeshObj* Enemy3 = pVAOManager->findMeshObjAddr("enemy3");
    Enemy3->Animation.tag = "PosLerpEaseOut";
    Enemy3->Animation.curSeq = 0;
    Enemy3->color_RGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
    Enemy3->Animation.Speed = 1.f;
    Enemy3->Animation.IsLooping = false;
    Enemy3->Animation.IsPlaying = false;
    Enemy3->Animation.AnimationTime = 0.f;
    g_pAnimationManager->animationOBJList.push_back(Enemy3);

    cMeshObj* Enemy4 = pVAOManager->findMeshObjAddr("enemy4");
    Enemy4->Animation.tag = "PosLerpEaseInOut";
    Enemy4->Animation.curSeq = 0;
    Enemy4->color_RGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
    Enemy4->Animation.Speed = 1.f;
    Enemy4->Animation.IsLooping = false;
    Enemy4->Animation.IsPlaying = false;
    Enemy4->Animation.AnimationTime = 0.f;
    g_pAnimationManager->animationOBJList.push_back(Enemy4);

    //create animation

    //Sequence 1
    AnimationData seq1_enemy1;
    seq1_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, 25.f), 0.f, None));
    seq1_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, None));
    seq1_enemy1.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, None));
    seq1_enemy1.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f,0.f,0.f,0.f), 0.f, None));
    seq1_enemy1.Duration = 5.f;
    g_pAnimationManager->AddAnimation("PosLerpNoEasing", seq1_enemy1);
    Enemy1->Animation.seq.push_back("PosLerpNoEasing");
    
    AnimationData seq1_enemy2;
    seq1_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, -25.f), 0.f, EaseIn));
    seq1_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseIn));
    seq1_enemy2.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseIn));
    seq1_enemy2.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseIn));
    seq1_enemy2.Duration = 5.f;
    g_pAnimationManager->AddAnimation("PosLerpEaseIn", seq1_enemy2);
    Enemy2->Animation.seq.push_back("PosLerpEaseIn");

    AnimationData seq1_enemy3;
    seq1_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, 25.f), 0.f, EaseOut));
    seq1_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseOut));
    seq1_enemy3.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseOut));
    seq1_enemy3.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseOut));
    seq1_enemy3.Duration = 5.f;
    g_pAnimationManager->AddAnimation("PosLerpEaseOut", seq1_enemy3);
    Enemy3->Animation.seq.push_back("PosLerpEaseOut");


    AnimationData seq1_enemy4;
    seq1_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, -25.f), 0.f, EaseInOut));
    seq1_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseInOut));
    seq1_enemy4.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseInOut));
    seq1_enemy4.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseInOut));
    seq1_enemy4.Duration = 5.f;
    g_pAnimationManager->AddAnimation("PosLerpEaseInOut", seq1_enemy4);
    Enemy4->Animation.seq.push_back("PosLerpEaseInOut");

    //sequence 2
    AnimationData seq2_enemy1;
    seq2_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 0.f, None));
    seq2_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, -25.f), 5.f, None));
    seq2_enemy1.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, None));
    seq2_enemy1.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 5.f, None));
    seq2_enemy1.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, None));
    seq2_enemy1.Duration = 5.f;
    g_pAnimationManager->AddAnimation("ScaleLerpNoEasing", seq2_enemy1);
    Enemy1->Animation.seq.push_back("ScaleLerpNoEasing");

    AnimationData seq2_enemy2;
    seq2_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 0.f, EaseIn));
    seq2_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, 25.f), 5.f, EaseIn));
    seq2_enemy2.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseIn));
    seq2_enemy2.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 5.f, EaseIn));
    seq2_enemy2.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseIn));
    seq2_enemy2.Duration = 5.f;
    g_pAnimationManager->AddAnimation("ScaleLerpEaseIn", seq2_enemy2);
    Enemy2->Animation.seq.push_back("ScaleLerpEaseIn");


    AnimationData seq2_enemy3;
    seq2_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 0.f, EaseOut));
    seq2_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, -25.f), 5.f, EaseOut));
    seq2_enemy3.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseOut));
    seq2_enemy3.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 5.f, EaseOut));
    seq2_enemy3.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseOut));
    seq2_enemy3.Duration = 5.f;
    g_pAnimationManager->AddAnimation("ScaleLerpEaseOut", seq2_enemy3);
    Enemy3->Animation.seq.push_back("ScaleLerpEaseOut");


    AnimationData seq2_enemy4;
    seq2_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 0.f, EaseInOut));
    seq2_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, 25.f), 5.f, EaseInOut));
    seq2_enemy4.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f, EaseInOut));
    seq2_enemy4.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 5.f, EaseInOut));
    seq2_enemy4.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 0.f), 0.f, EaseInOut));
    seq2_enemy4.Duration = 5.f;
    g_pAnimationManager->AddAnimation("ScaleLerpEaseInOut", seq2_enemy4);
    Enemy4->Animation.seq.push_back("ScaleLerpEaseInOut");


    //sequence 3
    AnimationData seq3_enemy1;
    seq3_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, -25.f), 0.f, None));
    seq3_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-12.5f, 25.f, -12.5f), 2.5f, None));
    seq3_enemy1.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, None));
    seq3_enemy1.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f, None));
    seq3_enemy1.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 5.f, None));
    seq3_enemy1.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 0.f, None));
    seq3_enemy1.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 1.f), 2.5f, None));
    seq3_enemy1.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 5.f, None));
    seq3_enemy1.Duration = 5.f;
    g_pAnimationManager->AddAnimation("RotationSlerpNoEasing", seq3_enemy1);
    Enemy1->Animation.seq.push_back("RotationSlerpNoEasing");

    AnimationData seq3_enemy2;
    seq3_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-25.f, 1.f, 25.f), 0.f, EaseIn));
    seq3_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-12.5f, 25.f, 12.5f), 2.5f, EaseIn));
    seq3_enemy2.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseIn));
    seq3_enemy2.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f, EaseIn));
    seq3_enemy2.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 5.f, EaseIn));
    seq3_enemy2.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 0.f, EaseIn));
    seq3_enemy2.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 1.f), 2.5f, EaseIn));
    seq3_enemy2.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 5.f, EaseIn));
    seq3_enemy2.Duration = 5.f;
    g_pAnimationManager->AddAnimation("RotationSlerpEaseIn", seq3_enemy2);
    Enemy2->Animation.seq.push_back("RotationSlerpEaseIn");


    AnimationData seq3_enemy3;
    seq3_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, -25.f), 0.f, EaseOut));
    seq3_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(12.5f, 25.f, -12.5f), 2.5f, EaseOut));
    seq3_enemy3.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseOut));
    seq3_enemy3.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f, EaseOut));
    seq3_enemy3.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 5.f, EaseOut));
    seq3_enemy3.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 0.f, EaseOut));
    seq3_enemy3.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 1.f), 2.5f, EaseOut));
    seq3_enemy3.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 5.f, EaseOut));
    seq3_enemy3.Duration = 5.f;
    g_pAnimationManager->AddAnimation("RotationSlerpEaseOut", seq3_enemy3);
    Enemy3->Animation.seq.push_back("RotationSlerpEaseOut");


    AnimationData seq3_enemy4;
    seq3_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(25.f, 1.f, 25.f), 0.f, EaseInOut));
    seq3_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(12.5f, 25.f, 12.5f), 2.5f, EaseInOut));
    seq3_enemy4.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 1.f, 0.f), 5.f, EaseInOut));
    seq3_enemy4.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f, EaseInOut));
    seq3_enemy4.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 5.f, EaseInOut));
    seq3_enemy4.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 0.f, EaseInOut));
    seq3_enemy4.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0.f, 0.f, 0.f, 1.f), 2.5f, EaseInOut));
    seq3_enemy4.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1.f, 0.f, 0.f, 0.f), 5.f, EaseInOut));
    seq3_enemy4.Duration = 5.f;
    g_pAnimationManager->AddAnimation("RotationSlerpEaseInOut", seq3_enemy4);
    Enemy4->Animation.seq.push_back("RotationSlerpEaseInOut");
}

void createPhysXscene(cVAOManager* pVAOManager)
{
    pVAOManager->setInstanceObjLighting("floor", false);
    g_physic->createFloor(pVAOManager->findMeshObjAddr("floor"));
    
    //stacked box
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::string instantName = "box" + std::to_string(i) + "_"+ std::to_string(j);
            pVAOManager->createOBJ("box", instantName, glm::vec3(i, j, 0), glm::vec3(0.5));
            pVAOManager->setInstanceObjRGB(instantName, glm::vec4(1, 0, 0, 1));
            pVAOManager->setInstanceObjLighting(instantName, true);
            g_physic->createBox(pVAOManager->findMeshObjAddr(instantName), glm::vec3(i, j+1, 16-i), glm::vec3(1));
        }
    }

    //static box
    for (int i = 1; i < 4; i++)
    {
        std::string instantName = "static_box" + std::to_string(i);
        pVAOManager->createOBJ("box", instantName, glm::vec3(i, 1, 0), glm::vec3(i*0.5));
        pVAOManager->setInstanceObjRGB(instantName, glm::vec4(1, 1, 1, 1));
        pVAOManager->setInstanceObjLighting(instantName, true);
        g_physic->createBox(pVAOManager->findMeshObjAddr(instantName), glm::vec3(i*(-4), i, i * (-4)), glm::vec3(i) , true);
    }

    //Bowling pin
    int count = 0;
    glm::vec3 offset = glm::vec3(-10,0,10);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < i+1; j++)
        {
            std::string instantName = "pin" + std::to_string(count);
            count++;
            glm::vec3 pos = glm::vec3((j * 2 - i) , 1, i );
            pVAOManager->createOBJ("pin", instantName, pos + offset, glm::vec3(0.02));
            pVAOManager->setTexture(instantName, "Dungeons_2_Texture_01_A.bmp", 0);
            pVAOManager->setTextureRatio(instantName, 0, 1);
            pVAOManager->setInstanceObjLighting(instantName, false);
            g_physic->createCylinder(pVAOManager->findMeshObjAddr(instantName), pos, glm::vec3(1));
        }
    }
    gVAOManager = pVAOManager;
    //g_physic->createBall(pVAOManager->findMeshObjAddr("ball1"), glm::vec3(0), 1);
//    g_physic->createBall(pVAOManager->findMeshObjAddr("ball2"), 2);
//    g_physic->createBall(pVAOManager->findMeshObjAddr("ball3"), 3);
//    g_physic->createBall(pVAOManager->findMeshObjAddr("ball4"), 4);
//    g_physic->createBall(pVAOManager->findMeshObjAddr("ball5"), 5);
}
