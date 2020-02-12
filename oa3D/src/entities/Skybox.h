#ifndef _Skybox_H
#define _Skybox_H

#include "Vertex.h"
#include "Entity.h"
#include "Texture.h"
#include "Camera.h"
#include <string>

using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::radians;
using std::vector;
using std::string;

static const vector<string> standard_skybox_faces
{
        "../oa3D/oa3D/assets/skyboxes/water/right.jpg",
        "../oa3D/oa3D/assets/skyboxes/water/left.jpg",
        "../oa3D/oa3D/assets/skyboxes/water/top.jpg",
        "../oa3D/oa3D/assets/skyboxes/water/bottom.jpg",
        "../oa3D/oa3D/assets/skyboxes/water/front.jpg",
        "../oa3D/oa3D/assets/skyboxes/water/back.jpg"
};

static const vector<string> yokohama_skybox_faces
{
        "../oa3D/oa3D/assets/skyboxes/yokohama/right.jpg",
        "../oa3D/oa3D/assets/skyboxes/yokohama/left.jpg",
        "../oa3D/oa3D/assets/skyboxes/yokohama/top.jpg",
        "../oa3D/oa3D/assets/skyboxes/yokohama/bottom.jpg",
        "../oa3D/oa3D/assets/skyboxes/yokohama/front.jpg",
        "../oa3D/oa3D/assets/skyboxes/yokohama/back.jpg"
};

class Skybox : public Entity
{


    public:

        Skybox(bool useStandard)
        : Entity()
        {
            if(useStandard)
                cubemapTexture = TextureUtil::LoadCubeMap(standard_skybox_faces);
            else
                cubemapTexture = TextureUtil::LoadCubeMap(yokohama_skybox_faces);

            bindSkybox();
        }

        void bindSkybox()
        {
            float skyboxVertices[] = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };

            glGenVertexArrays(1, &skyboxVAO);
            glGenBuffers(1, &skyboxVBO);
            glBindVertexArray(skyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW); //vertices[0] referes to Vertex.Position (first element)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);;
        }

        void drawSkyBox(Camera *camera, Shader *skyShader)
        {
            skyShader->Use();
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            mat4 view = mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
            skyShader->setMat4("view", view);
            skyShader->setMat4("projection", camera->GetPerspective());

            // skybox cube
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }


    private:
        unsigned int skyboxVAO;
        unsigned int skyboxVBO;
        unsigned int cubemapTexture;


};



#endif // _Skybox_H

