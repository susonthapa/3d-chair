#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Shader.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow*,int,int);
void processInput(GLFWwindow*,int ,int,int,int);

//Screen Width and height
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Rotation about X,Y and Z axis
float rotX=0.0f,rotY=1.0f,rotZ=0.0f;

//Zoom Coordinates
float transZ=-3.0f,transX=0.0f;

//Ambient Light Strength
float ambStrength=0.3f;

//Class to handle Buffers for open gl
class BufferHandler {

 public:
     //Function to setup Position ,Texture and Normal Data Buffers
     static void setupBuffers(unsigned int *vboHandle,float *positionData,float *normalData,float *textureData,int sizes) {
         //Generate 3 Buffers
        glGenBuffers(3,vboHandle);

        //Bind Position Data Buffer to vboHandl[0]
        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[0]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*sizes,positionData,GL_STATIC_DRAW);

        //Bind Normal Data Buffer to vboHandle[1]
        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[1]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*sizes,normalData,GL_STATIC_DRAW);

        //Bind Texture Data Buffer to vboHandle[2]
        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[2]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*72,textureData,GL_STATIC_DRAW);

     }

     //Function to bind and load the required Buffers
     static void bindAndLoadBuffer(unsigned int *vboHandle,unsigned int vaoHandle) {
         //Bind the Vertex Buffer Object
        glBindVertexArray(vaoHandle);

        //Bind the Buffers and set the stride and data values
        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[0]);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);//Enable the current Buffer

        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[1]);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER,vboHandle[2]);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
        glEnableVertexAttribArray(2);
     }

     //Function to set the Texture for the Shapes
     static void setupTexture(unsigned int *textureHandle) {
         //Generate and Bind Textures Objects
        glGenTextures(1,textureHandle);
        glBindTexture(GL_TEXTURE_2D,textureHandle[0]);

        //Set Various Texture Parameters
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     }

     //Function to load the Texture
     static void loadTexture(unsigned int *textureHandle) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureHandle[0]);
     }

};

//Class to Represent A Cube
class Cube {
private:
    //Cube Vertex Data
    float positionData[108]={
        //Back Face

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        //Front Face

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        //Left Face

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        //Right Face

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

         //Bottom Face

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        //Top Face

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    //Setup the Normal For each Face
    float normalData[108] {

        //Back Face Normal

        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,

        //Front Face Normal
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,

        //Left Face Normal
        -1.0f,0.0f,0.0f,
        -1.0f,0.0f,0.0f,
        -1.0f,0.0f,0.0f,
        -1.0f,0.0f,0.0f,
        -1.0f,0.0f,0.0f,
        -1.0f,0.0f,0.0f,

        //Right Face Normal
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        //Bottom Face Normal
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,

        //Top Face Normal
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f

    };

    //Setup The Texture for each Face
    float textureData[72]={
        //Back Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        //Front Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        //Left Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        //Right Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        //Bottom Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        //TOp Face Texture
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        1.0f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f
    };

    float x,y,z,l,b,h;

public:
    Cube() {

    }

    //Function to setup The Cube center and length,breadth and height
    void cubeSetup(float x,float y,float z,float l,float b,float h) {
        this->x=x;
        this->y=y;
        this->z=z;
        this->l=l;
        this->b=b;
        this->h=h;
        setupFrontFace();
        setupBackFace();
        setupLeftFace();
        setupRightFace();
        setupTopFace();
        setupBottomFace();
    }

    //Helpers Functions to Setup Various Faces of Cube
private:
    void setupFrontFace() {
        float frontFace[]={
            x,y,z+b,
            x+l,y,z+b,
            x+l,y+h,z+b,
            x+l,y+h,z+b,
            x,y,z+b,
            x,y+h,z+b

        };

        arrayCopy(frontFace,positionData,0);
    }

    void setupBackFace() {
        float backFace[]={
            x,y,z,
            x+l,y,z,
            x+l,y+h,z,
            x,y,z,


            x+l,y+h,z,
            x,y+h,z
        };
        arrayCopy(backFace,positionData,18);
    }

    void setupLeftFace() {
        float leftFace[]={
            x,y,z,

            x,y+h,z,
            x,y+h,z+b,
            x,y,z,
            x,y+h,z+b,
            x,y,z+b
        };
        arrayCopy(leftFace,positionData,36);
    }

    void setupRightFace() {
        float rightFace[]={

            x+l,y,z+b,
            x+l,y,z,
            x+l,y+h,z,
            x+l,y,z+b,
            x+l,y+h,z,
            x+l,y+h,z+b
        };

        arrayCopy(rightFace,positionData,54);
    }
    void setupTopFace() {
        float topFace[]={

            x+l,y+h,z+b,
            x+l,y+h,z,
            x,y+h,z,
            x+l,y+h,z+b,
            x,y+h,z,
            x,y+h,z+b
        };
        arrayCopy(topFace,positionData,72);
    }

    void setupBottomFace() {
        float bottomFace[]={

            x,y,z+b,
            x+l,y,z+b,
            x+l,y,z,
            x,y,z+b,
            x+l,y,z,
            x,y,z
        };
        arrayCopy(bottomFace,positionData,90);
    }


    //Function to copy one array to another
    void arrayCopy(float *sourceData,float *destinationData,int startIndex) {
        for(int i=0;i<18;i++) {
            destinationData[startIndex+i]=sourceData[i];
        }
    }
    //Getter Functions
public:
    float* getPositionData() {
        return positionData;
    }

    float* getNormalData() {
        return normalData;
    }

    float* getTextureData() {
        return textureData;
    }

};

int main()
{
    //Initialize GLFW and setup our Opengl Versions and Profiles
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Transformation of Chair", NULL, NULL);
    if (window == NULL)//Check for window creation
    {
        cout << "Failed to create GLFW window"<<endl;
        glfwTerminate();
        return -1;
    }
    //Set Current GL Context
    glfwMakeContextCurrent(window);

    //Change in window size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //CallBack for key pressed
    glfwSetKeyCallback(window,processInput);

    //Glad Loader to link the open gl library
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" <<endl;
        return -1;
    }
    int width,height,nrChannels;

    //Load Our Texture Data
    unsigned char *data=stbi_load("texture.jpg",&width,&height,&nrChannels,0);

    //Compile and Link our Shader
    Shader ourShader("vector_shader.glsl", "fragment_shader.glsl");

    //Vertex Array Object Handle
    unsigned int VAO;

    //Total No of Cubes in Chair=20;
    int cubeCount=20;

    //Buffer for each Cube=3
    int bufferCount=3;

    //Texture for each Cube=1
    unsigned int textureCount[cubeCount][1];

    //Total Buffers for all the cubes
    unsigned int cubes[cubeCount][bufferCount];

    //Initialize the Cube Objects
    Cube cube[cubeCount];

    //Setup legs and other surfaces of chair
    //Back Support Left Pillar Note another pillar to the last
    cube[0].cubeSetup(-0.3f,0.175f,-0.1f,0.1f,0.05f,0.7f);
    //Seat
    cube[1].cubeSetup(-0.3f,0.1f,-0.1f,0.6f,0.7f,0.075f);

    //Leg 1
    cube[2].cubeSetup(-0.3f,-0.5f,0.52f,0.08f,0.08f,0.6f);
    //Leg 2
    cube[3].cubeSetup(0.22f,-0.5f,0.52f,0.08f,0.08f,0.6f);
    //Leg 3
    cube[4].cubeSetup(-0.3f,-0.5f,-0.1f,0.08f,0.08f,0.6f);
    //Leg 4
    cube[5].cubeSetup(0.22f,-0.5f,-0.1f,0.08f,0.08f,0.6f);

    //Left Side Support
    cube[6].cubeSetup(-0.3f,0.375f,-0.05f,0.075f,0.65f,0.075f);
    //Left Side Support Vertical
    cube[7].cubeSetup(-0.3f,0.175f,0.525f,0.075f,0.075f,0.2f);
    //Right Side Support
    cube[8].cubeSetup(0.225f,0.375f,-0.05f,0.075f,0.65f,0.075f);
    //Right Side Support Vertical
    cube[9].cubeSetup(0.225f,0.175f,0.525f,0.075f,0.075f,0.2f);
    
    //Left Leg Support
    cube[10].cubeSetup(-0.3f,-0.4f,-0.02f,0.05f,0.54f,0.05f);
    //Right Leg Support
    cube[11].cubeSetup(0.25f,-0.4f,-0.02f,0.05f,0.54f,0.05f);
    //Back Leg Support
    cube[12].cubeSetup(-0.22f,-0.1f,-0.1f,0.44f,0.05f,0.05f);
    //Front Leg Support
    cube[13].cubeSetup(-0.22f,-0.1f,0.55f,0.44f,0.05f,0.05f);

    //Continue Pillar Construction
    //Right Support Pillar
    cube[14].cubeSetup(0.2f,0.175f,-0.1f,0.1f,0.05f,0.7f);
    //Support Pillar Horizontal Close
    cube[15].cubeSetup(-0.3f,0.875f,-0.1f,0.6f,0.05f,0.1f);

    //Back Support Horizontal Pillars
    //First Horizontal Pillar
    cube[16].cubeSetup(-0.2f,0.29f,-0.1f,0.4f,0.05f,0.08f);
    //Second Horizontal Pillar
    cube[17].cubeSetup(-0.2f,0.42f,-0.1f,0.4f,0.05f,0.08f);
    //Third Horizontal Pillar
    cube[18].cubeSetup(-0.2f,0.55f,-0.1f,0.4f,0.05f,0.08f);
    //Fourth Horizontal Pillar
    cube[19].cubeSetup(-0.2f,0.68f,-0.1f,0.4f,0.05f,0.08f);

    //Enable Depth Test for visible Surface Detection
    glEnable(GL_DEPTH_TEST);

    //Generate VAO
    glGenVertexArrays(1,&VAO);

    //Bind the VAO
    glBindVertexArray(VAO);

    //Setup Buffers and Texture for all the cubes
    for(int i=0;i<cubeCount;i++) {
        BufferHandler::setupBuffers(cubes[i],cube[i].getPositionData(),cube[i].getNormalData(),cube[i].getTextureData(),108);
        BufferHandler::setupTexture(textureCount[i]);
    }

    //Check if Our Texture is Loaded
    if(data) {
        cout<<"Texture Load Successfully"<<endl;
        //Link the texture with minmaps
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout<<"Data Load Failed"<<endl;
    }

    //Free the image
    stbi_image_free(data);

    //Use our Shader Program
    ourShader.use();

    //Set the texture identifier
    ourShader.setInt("texture1",0);

    int i;
    //Main Render Loop
    while (!glfwWindowShouldClose(window))
    {
        //Set the Current Screen Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //Clear the Previous Color and Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //Bind all the textures
        for(i=0;i<cubeCount;i++) {
            BufferHandler::loadTexture(textureCount[i]);
        }

        //Use The Current Open GL program
        ourShader.use();

        //Model ,View and Projection Matrices
        glm::mat4 model,view,projection;
        // initialize to identity
        model = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);

        //Rotation Matrix Generation
        model=glm::rotate(model,(float)glfwGetTime(),glm::vec3(rotX,rotY,rotZ));
        view=glm::translate(view,glm::vec3(transX,0.0f,transZ));
        projection=glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);

        //Load the matrices to vertex shader
        ourShader.setMat4("model",model);
        ourShader.setMat4("view",view);
        ourShader.setMat4("projection",projection);

        ourShader.setFloat("ambLightStrength",ambStrength);

        //Draw all the cubes
        for(i=0;i<cubeCount;i++) {
            BufferHandler::bindAndLoadBuffer(cubes[i],VAO);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        //For Double Buffering
        glfwSwapBuffers(window);

        //Check for any window close events
        glfwPollEvents();
    }

    //Delete the VBO and release GPU Memory
    glDeleteVertexArrays(1, &VAO);

    //Close GLFW Window
    glfwTerminate();
    return 0;
}

//Function to Process the KeyBoard Input
void processInput(GLFWwindow *window,int key,int scancode,int action,int mods)
{
    if(action==GLFW_PRESS||action==GLFW_REPEAT) {
        switch(key) {
        case GLFW_KEY_LEFT:
            if(rotX>-1) {
                rotX-=0.025;
            }
            break;
        case GLFW_KEY_RIGHT:
            if(rotX<1) {
                rotX+=0.025;
            }
            break;
        case GLFW_KEY_UP:
            if(rotZ>-1) {
                rotZ-=0.025;
            }
            break;
        case GLFW_KEY_DOWN:
            if(rotZ<1) {
                rotZ+=0.025;
            }
            break;
        case GLFW_KEY_W:
            if(transZ>-4)
                transZ-=0.05;
            break;
        case GLFW_KEY_S:
            if(transZ<0.5)
                transZ+=0.05;
            break;
        case GLFW_KEY_A:
            if(transX>-1.25)
                transX-=0.05;
            break;
        case GLFW_KEY_D:
            if(transX<1.25)
                transX+=0.05;
            break;
        case GLFW_KEY_KP_ADD:
            if(ambStrength<1)
                ambStrength+=0.05;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            if(ambStrength>0)
                ambStrength-=0.05;
            break;
        }
    }
}

//Function to resize the ViewPort
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
