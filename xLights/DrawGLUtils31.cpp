
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __WXMAC__
#include "OpenGL/gl3.h"
#define __gl_h_
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#ifdef _MSC_VER
#include "GL/glut.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#endif

#include <DrawGLUtils.h>
#include <stack>

class OpenGL31Cache : public DrawGLUtils::xlGLCacheInfo {
public:


    OpenGL31Cache() : matrix(nullptr) {
        max = 1024;
        colors = new float[max * 4];
        vertices = new float[max * 3];
        curCount = 0;
        
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        GLint Result = GL_FALSE;
        int InfoLogLength;
        const char *VertexSourcePointer =
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
            "layout(location = 1) in vec4 vertexColor;\n"
            "layout(location = 2) in vec2 vertexUV;\n"
            "out vec4 fragmentColor;\n"
            "out vec2 UV;\n"
            "uniform mat4 MVP;\n"
            "void main(){\n"
            "    gl_Position = MVP * vec4(vertexPosition_modelspace,1);"
            "    fragmentColor = vertexColor;\n"
            "    UV = vertexUV;\n"
            "}\n";
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);
        
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        }
        
        char const * FragmentSourcePointer = "#version 330 core\n"
        "in vec4 fragmentColor;\n"
        "in vec2 UV;\n"
        "in float radius;\n"
        "out vec4 color;\n"
        "uniform sampler2D myTextureSampler;\n"
        "uniform int RenderType;\n"
        "void main(){\n"
        "    if (RenderType == -1) {\n"
        "        color = texture(myTextureSampler, UV);\n"
        "    } else if (RenderType == 0) {\n"
        "        color = fragmentColor;\n"
        "    } else {\n"
        "        vec2 dv = abs(gl_PointCoord - vec2(0.5, 0.5));\n "
        "        float dist = distance(dv, vec2(0,0));\n"
        "        float alpha = smoothstep(0.5, 0.35, dist);\n"
        "        color = vec4(fragmentColor.x, fragmentColor.y, fragmentColor.z, alpha);\n"
        "    }\n"
        "}\n";
        
        
        /*
         "        float dist = distance(gl_FragCoord.xy, fPosition);\n"
         "        float delta = fwidth(dist);\n"
         "        float alpha = smoothstep(0.45 - delta, 0.45, dist);\n"
         "        color = mix(fragmentColor, vec4(0.0), alpha);\n"
         */
        
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);
        
        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        }
        
        ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        
        glLinkProgram(ProgramID);
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        }
        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        glUseProgram(ProgramID);
        
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        glGenBuffers(1, &vBufferId);
        glGenBuffers(1, &cBufferId);
        MatrixID = glGetUniformLocation(ProgramID, "MVP");
        TextureID = glGetUniformLocation(ProgramID, "myTextureSampler");
        RenderTypeID = glGetUniformLocation(ProgramID, "RenderType");
    }
    ~OpenGL31Cache() {
        if (matrix) {
            delete matrix;
        }
        while (!matrixStack.empty()) {
            delete matrixStack.top();
            matrixStack.pop();
        }
        if (ProgramID != 0) {
            glDeleteBuffers(1, &vBufferId);
            glDeleteBuffers(1, &cBufferId);
            glDeleteVertexArrays(1, &VertexArrayID);
            glDeleteProgram(ProgramID);
        }
        delete [] colors;
        delete [] vertices;
    }
    virtual void addVertex(double x, double y, const xlColor &c) override {
        ensureSize(1);
        vertices[curCount * 3] = x;
        vertices[curCount * 3 + 1] = y;
        vertices[curCount * 3 + 2] = 0.0;
        colors[curCount*4] = ((float)c.Red())/255.0;
        colors[curCount*4 + 1] = ((float)c.Green())/255.0;
        colors[curCount*4 + 2] = ((float)c.Blue())/255.0;
        colors[curCount*4 + 3] = ((float)c.Alpha())/255.0;
        curCount++;
    }
    virtual void ensureSize(int s) override {
        int size = curCount + s;
        if (size > max) {
            float *tmp = new float[size * 4];
            for (int x = 0; x < (max*4); x++) {
                tmp[x] = colors[x];
            }
            delete [] colors;
            colors = tmp;
            float *tmpf = new float[size * 3];
            for (int x = 0; x < (max*3); x++) {
                tmpf[x] = vertices[x];
            }
            delete [] vertices;
            vertices = tmpf;
            max = size;
        }
    }
    virtual int vertexCount() override {
        return curCount;
    }
    
    void flush(int type) override {
        
        glBindBuffer(GL_ARRAY_BUFFER, vBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*curCount*3, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, cBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*curCount*4, colors, GL_STATIC_DRAW);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vBufferId);
        glVertexAttribPointer(
                              0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cBufferId);
        glVertexAttribPointer(1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                              4,                                // size
                              GL_FLOAT,                 // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );

        glUniform1i(RenderTypeID, type == GL_POINTS ? 1 : 0);
        glDrawArrays(type, 0, curCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        curCount = 0;
    }
    void Ortho(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y) override {
        if (matrix) {
            delete matrix;
        }
        glm::mat4 m = glm::ortho((float)topleft_x, (float)bottomright_x, (float)bottomright_y, (float)topleft_y);
        matrix = new glm::mat4(m);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m));
    }
    void PushMatrix() override {
        matrixStack.push(matrix);
        matrix = new glm::mat4(*matrix);
    }
    void PopMatrix() override {
        if (!matrixStack.empty()) {
            if (matrix != nullptr) {
                delete matrix;
            }
            matrix = matrixStack.top();
            matrixStack.pop();
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(*matrix));
        }
    }
    void Translate(float x, float y, float z) override {
        if (matrix) {
            glm::mat4 tm = glm::translate(*matrix, glm::vec3(x, y, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            glm::mat4 tm = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            matrix = new glm::mat4(tm);
        }
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(*matrix));
    }
    void Rotate(float angle, float x, float y, float z) override {
        if (matrix) {
            angle = wxDegToRad(angle);
            glm::mat4 tm = glm::rotate(*matrix, angle, glm::vec3(x, y, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            angle = wxDegToRad(angle);
            glm::mat4 tm = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z));
            matrix = new glm::mat4(tm);
        }
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(*matrix));
    }
    void Scale(float w, float h, float z) override {
        if (matrix) {
            glm::mat4 tm = glm::scale(*matrix, glm::vec3(w, h, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            glm::mat4 tm = glm::scale(glm::mat4(1.0f), glm::vec3(w, h, z));
            matrix = new glm::mat4(tm);
        }
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(*matrix));
    }


protected:
    int max;
    float *colors;
    float *vertices;
    int curCount;

    GLuint MatrixID;
    GLuint TextureID;
    GLuint RenderTypeID;
    
    GLuint ProgramID;
    GLuint vBufferId;
    GLuint cBufferId;
    GLuint VertexArrayID;

    std::stack<glm::mat4*> matrixStack;
    
    glm::mat4 *matrix;
};



DrawGLUtils::xlGLCacheInfo *Create31Cache() {
    return new OpenGL31Cache();
}