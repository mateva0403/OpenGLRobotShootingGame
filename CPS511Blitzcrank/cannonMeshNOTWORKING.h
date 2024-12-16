#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
NOTE: ORIGINALLY, THIS WAS HOW I WOULD BE IMPORTING MY MESH. HOWEVER, FOR THE SAME REASON I DID NOT SUBMIT ASSIGNMENT 2 WE ARE STUCK
TRYING TO LOAD VALUES INTO THE VAO/VBO. THERE IS ALWAYS SOME ERROR.

I WOULD REALLY APPRECIATE (IF YOU HAVE THE TIME) TO MAYBE GIVE FEEDBACK ON HOW TO FIX THIS, FOR FUTURE REFERENCE.
ORIGINALLY DID AS2 AS WELL, HOWEVER UNSUBMITTED DUE TO SOME MEMORY ACCESS ERROR SIMILAR TO HERE, HENCE
MY OWN CUSTOM MESH INCLUDED HERE.

THANKS!
*/

// Maximum sizes
#define MAX_VERTICES 528
#define MAX_QUADS 512
#define MAX_INDICES 2048

// Vertex
typedef struct Vertex {
    GLdouble x, y, z;     
    struct { GLdouble x, y, z; } normal; 
} Vertex;

// Qua structure
typedef struct Quad {
    int vertexIndex[4]; 
} Quad;

Vertex* varray = NULL;
Quad* qarray = NULL;
unsigned int indices[MAX_INDICES];
int numVertices = 0;
int numQuads = 0;
int numIndices = 0;

// OpenGL objects
GLuint cannonVAO, cannonVertexVBO, cannonNormalVBO, cannonIndexVBO;

void loadCannonOBJ(const char* filename) 
    {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || !file) {
        printf("Error opening OBJ file: %s\n", filename);
        return;
    }

    varray = (Vertex*)malloc(MAX_VERTICES * sizeof(Vertex));
    qarray = (Quad*)malloc(MAX_QUADS * sizeof(Quad));
    if (!varray || !qarray) 
    {
        printf("Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Temporary storage
    GLdouble tempVertices[MAX_VERTICES][3];
    GLdouble tempNormals[MAX_VERTICES][3];
    int tempQuads[MAX_QUADS][4];

    int vCount = 0, nCount = 0, qCount = 0, iCount = 0;
    char line[128];

    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "v ", 2) == 0) 
        {
            if (vCount >= MAX_VERTICES) 
            {
                printf("Error: Too many vertices\n");
                break;
            }
            sscanf_s(line, "v %lf %lf %lf",
                &tempVertices[vCount][0],
                &tempVertices[vCount][1],
                &tempVertices[vCount][2]);
            vCount++;
        }
        else if (strncmp(line, "vn ", 3) == 0) 
        {
            if (nCount >= MAX_VERTICES) 
            {
                printf("Error: Too many normals\n");
                break;
            }
            sscanf_s(line, "vn %lf %lf %lf",
                &tempNormals[nCount][0],
                &tempNormals[nCount][1],
                &tempNormals[nCount][2]);
            nCount++;
        }
        else if (strncmp(line, "f ", 2) == 0) 
        {
            if (qCount >= MAX_QUADS || iCount + 4 > MAX_INDICES) 
            {
                printf("Error: Too many quads or indices\n");
                break;
            }

            int indices[8];
            int parsed = sscanf_s(line, "f %d//%d %d//%d %d//%d %d//%d",
                &indices[0], &indices[1],
                &indices[2], &indices[3],
                &indices[4], &indices[5],
                &indices[6], &indices[7]);

            if (parsed < 8) 
            {
                printf("Error: Invalid face format in line: %s\n", line);
                continue;
            }

            // Convert 1-based to 0-based and validate
            for (int j = 0; j < 4; j++) {
                indices[j * 2] -= 1; // Vertex index
                if (indices[j * 2] < 0 || indices[j * 2] >= numVertices) {
                    printf("Error: Invalid vertex index %d in face at quad %d\n", indices[j * 2], qCount);
                    break;
                }
                tempQuads[qCount][j] = indices[j * 2];
            }
            qCount++;
        }

       
    }
    fclose(file);

    // Populate varray and qarray
    numVertices = vCount;
    numQuads = qCount;
    numIndices = iCount;

    for (int i = 0; i < numVertices; i++) 
    {
        varray[i].x = tempVertices[i][0];
        varray[i].y = tempVertices[i][1];
        varray[i].z = tempVertices[i][2];
    }

    for (int i = 0; i < numQuads; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            qarray[i].vertexIndex[j] = tempQuads[i][j];
        }
    }
    qarray = (Quad*)malloc(MAX_QUADS * sizeof(Quad));
    if (!varray) 
    {
        printf("Error: Failed to allocate memory for varray\n");

    }
    if (!qarray) 
    {
        printf("Error: Failed to allocate memory for qarray\n");

    }

}


void initCannonVAO() 
{

    glGenVertexArrays(1, &cannonVAO);
    glBindVertexArray(cannonVAO);

    glGenBuffers(1, &cannonVertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cannonVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, varray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Index
    glGenBuffers(1, &cannonIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cannonIndexVBO);
    
    GLuint* flatIndexBuffer = (GLuint*)malloc(sizeof(GLuint) * numIndices);
    if (!flatIndexBuffer) 
    {
        return;
    }
    if (!qarray) 
    {
        free(flatIndexBuffer);
        return;
    }

    for (int i = 0; i < numQuads; i++) 
    {
        if (i * 4 + 3 >= numIndices) 
        {
            break;
        }
        flatIndexBuffer[i * 4 + 0] = qarray[i].vertexIndex[0];
        flatIndexBuffer[i * 4 + 1] = qarray[i].vertexIndex[1];
        flatIndexBuffer[i * 4 + 2] = qarray[i].vertexIndex[2];
        flatIndexBuffer[i * 4 + 3] = qarray[i].vertexIndex[3];
    }

    // AN INDEXBUFFER WAS MY ATTEMPT AT AS2, IT WORKED BUT DOESN'T HERE. DUNNO WHY.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, flatIndexBuffer, GL_STATIC_DRAW);

    free(flatIndexBuffer);
    glBindVertexArray(0);
    
    for (int i = 0; i < numVertices; i++) 
    {
        printf("varray[%d]: %.2f %.2f %.2f\n", i, varray[i].x, varray[i].y, varray[i].z);
    }
}


void renderCannon() 
{
    glBindVertexArray(cannonVAO);
    glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
