#include <stdlib.h>

#include <stdio.h>

#include <math.h>

#include <GL/glut.h>

#include <GL/glu.h>





GLUquadric *quadSphere;

int swCull; /* toggles software culling, control with the <space> key */



/* Location of the lookat and camera objects, respectively */

float lookat[3], camera[3];









/*

 * matrix and math utility routines and macros

 */



static void matrixConcatenate (float *result, float *ma, float *mb)

{

    int i;

    double mb00, mb01, mb02, mb03,

           mb10, mb11, mb12, mb13,

           mb20, mb21, mb22, mb23,

           mb30, mb31, mb32, mb33;

    double mai0, mai1, mai2, mai3;



    mb00 = mb[0];  mb01 = mb[1];

    mb02 = mb[2];  mb03 = mb[3];

    mb10 = mb[4];  mb11 = mb[5];

    mb12 = mb[6];  mb13 = mb[7];

    mb20 = mb[8];  mb21 = mb[9];

    mb22 = mb[10];  mb23 = mb[11];

    mb30 = mb[12];  mb31 = mb[13];

    mb32 = mb[14];  mb33 = mb[15];



    for (i = 0; i < 4; i++) {

        mai0 = ma[i*4+0];  mai1 = ma[i*4+1];

	    mai2 = ma[i*4+2];  mai3 = ma[i*4+3];



        result[i*4+0] = mai0 * mb00 + mai1 * mb10 + mai2 * mb20 + mai3 * mb30;

        result[i*4+1] = mai0 * mb01 + mai1 * mb11 + mai2 * mb21 + mai3 * mb31;

        result[i*4+2] = mai0 * mb02 + mai1 * mb12 + mai2 * mb22 + mai3 * mb32;

        result[i*4+3] = mai0 * mb03 + mai1 * mb13 + mai2 * mb23 + mai3 * mb33;

    }

}



#define vectorLength(nin) \
    sqrt((nin)[0]*(nin)[0] + (nin)[1]*(nin)[1] + (nin)[2]*(nin)[2])

#define distanceFromPlane(peq,p) \
    ((peq)[0]*(p)[0] + (peq)[1]*(p)[1] + (peq)[2]*(p)[2] + (peq)[3])









/*

 * Wiew volume plane storage and calculation

 */



/* Storage for the six planes, left right top bottom near far */

float planeEqs[6][4];



/* Calculates the six view volume planes in object coordinate (OC) space.

       

       Algorithm:

       

       A view volume plane in OC is transformed into CC by multiplying it by

       the inverse of the combined ModelView and Projection matrix (M).

       Algebraically, this is written:

              -1

         P   M   = P

          oc        cc

       

       The resulting six view volume planes in CC are:

         [ -1  0  0  1 ]

         [  1  0  0  1 ]

         [  0 -1  0  1 ]

         [  0  1  0  1 ]

         [  0  0 -1  1 ]

         [  0  0  1  1 ]

       

       To transform the CC view volume planes into OC, we simply multiply

       the CC plane equations by the combined ModelView and Projection matrices

       using standard vector-matrix multiplication. Algebraically, this is written:  

         P   M = P

          cc      oc

       

       Since all of the CC plane equation components are 0, 1, or -1, full vector-

       matrix multiplication is overkill. For example, the first element of the

       first OC plane equation is computed as:

         A = -1 * m0 + 0 * m1 + 0 * m2 + 1 * m3

       This simplifies to:

         A = m3 - m0

       

       Other terms simpliofy similarly. In fact, all six plane equations can be

       computed as follows:

         [ m3-m0  m7-m4  m11-m8  m15-m12 ]

         [ m3+m0  m7+m4  m11+m8  m15+m12 ]

         [ m3-m1  m7-m5  m11-m9  m15-m13 ]

         [ m3+m1  m7+m5  m11+m9  m15+m13 ]

         [ m3-m2  m7-m6  m11-m10 m15-m14 ]

         [ m3+m2  m7+m6  m11+m10 m15+m14 ]

     */

static void calcViewVolumePlanes ()

{

    GLfloat ocEcMat[16], ecCcMat[16], ocCcMat[16];





    /* Get the modelview and projection matrices */

    glGetFloatv (GL_MODELVIEW_MATRIX, ocEcMat);

    glGetFloatv (GL_PROJECTION_MATRIX, ecCcMat);



    /* ocCcMat transforms from OC (object coordinates) to CC (clip coordinates) */

    matrixConcatenate (ocCcMat, ocEcMat, ecCcMat);



    /* Calculate the six OC plane equations. */

    planeEqs[0][0] = ocCcMat[3] - ocCcMat[0]; 

    planeEqs[0][1] = ocCcMat[7] - ocCcMat[4]; 

    planeEqs[0][2] = ocCcMat[11] - ocCcMat[8]; 

    planeEqs[0][3] = ocCcMat[15] - ocCcMat[12]; 



    planeEqs[1][0] = ocCcMat[3] + ocCcMat[0]; 

    planeEqs[1][1] = ocCcMat[7] + ocCcMat[4]; 

    planeEqs[1][2] = ocCcMat[11] + ocCcMat[8]; 

    planeEqs[1][3] = ocCcMat[15] + ocCcMat[12]; 



    planeEqs[2][0] = ocCcMat[3] + ocCcMat[1]; 

    planeEqs[2][1] = ocCcMat[7] + ocCcMat[5]; 

    planeEqs[2][2] = ocCcMat[11] + ocCcMat[9]; 

    planeEqs[2][3] = ocCcMat[15] + ocCcMat[13]; 



    planeEqs[3][0] = ocCcMat[3] - ocCcMat[1]; 

    planeEqs[3][1] = ocCcMat[7] - ocCcMat[5]; 

    planeEqs[3][2] = ocCcMat[11] - ocCcMat[9]; 

    planeEqs[3][3] = ocCcMat[15] - ocCcMat[13]; 



    planeEqs[4][0] = ocCcMat[3] + ocCcMat[2]; 

    planeEqs[4][1] = ocCcMat[7] + ocCcMat[6]; 

    planeEqs[4][2] = ocCcMat[11] + ocCcMat[10]; 

    planeEqs[4][3] = ocCcMat[15] + ocCcMat[14]; 



    planeEqs[5][0] = ocCcMat[3] - ocCcMat[2]; 

    planeEqs[5][1] = ocCcMat[7] - ocCcMat[6]; 

    planeEqs[5][2] = ocCcMat[11] - ocCcMat[10]; 

    planeEqs[5][3] = ocCcMat[15] - ocCcMat[14]; 

}









/*

 * Geometric data storage and management routines

 */



/* Structure for storing data about a sphere */

typedef struct {

    float trans[3];

    float radius;

    float color[3];

    float bbox[8][3];

} sphereStr;

sphereStr sphereData[4]; /* We draw four spheres */



/* Test a sphere's bounding box against the six clip planes */

static int culled (sphereStr sd)

{

    int i, j;

    int culled;



    for (i=0; i<6; i++) {

        culled = 0;

        for (j=0; j<8; j++) {

            if (distanceFromPlane(planeEqs[i], sd.bbox[j]) < 0.)

                culled |= 1<<j;

        }

        if (culled==0xff)

            /* All eight vertices of bounding box are trivially culled */

            return 1;

    }

    /* Not trivially culled. Probably visible. */

    return 0;

}



/* Given a sphere's radius and location, calculate its bounding box */

static void calcBBox (sphereStr *sd)

{

    sd->bbox[0][0] = sd->bbox[1][0] = 

        sd->bbox[2][0] = sd->bbox[3][0] = sd->trans[0]-sd->radius;

    sd->bbox[4][0] = sd->bbox[5][0] = 

        sd->bbox[6][0] = sd->bbox[7][0] = sd->trans[0]+sd->radius;

    

    sd->bbox[2][1] = sd->bbox[3][1] = 

        sd->bbox[6][1] = sd->bbox[7][1] = sd->trans[1]-sd->radius;

    sd->bbox[0][1] = sd->bbox[1][1] = 

        sd->bbox[4][1] = sd->bbox[5][1] = sd->trans[1]+sd->radius;

    

    sd->bbox[0][2] = sd->bbox[2][2] = 

        sd->bbox[4][2] = sd->bbox[6][2] = sd->trans[2]-sd->radius;

    sd->bbox[1][2] = sd->bbox[3][2] = 

        sd->bbox[5][2] = sd->bbox[7][2] = sd->trans[2]+sd->radius;

}



/* Init our four spheres */

static void initSpheres ()

{

    sphereData[0].trans[0] = -3.;

    sphereData[0].trans[1] = 0.;

    sphereData[0].trans[2] = 0.;

    sphereData[0].radius = .5;

    sphereData[0].color[0] = 1.;

    sphereData[0].color[1] = 0.;

    sphereData[0].color[2] = 0.;

    calcBBox (&sphereData[0]);

    

    sphereData[1].trans[0] = 0.;

    sphereData[1].trans[1] = -1.;

    sphereData[1].trans[2] = 0.;

    sphereData[1].radius = .5;

    sphereData[1].color[0] = 0.;

    sphereData[1].color[1] = 1.;

    sphereData[1].color[2] = 0.;

    calcBBox (&sphereData[1]);

    

    sphereData[2].trans[0] = 0.;

    sphereData[2].trans[1] = 0.;

    sphereData[2].trans[2] = -2.;

    sphereData[2].radius = .5;

    sphereData[2].color[0] = 0.5;

    sphereData[2].color[1] = 0.5;

    sphereData[2].color[2] = 1.;

    calcBBox (&sphereData[2]);

    

    sphereData[3].trans[0] = 4.;

    sphereData[3].trans[1] = 0.;

    sphereData[3].trans[2] = 0.;

    sphereData[3].radius = .5;

    sphereData[3].color[0] = 1.;

    sphereData[3].color[1] = 1.;

    sphereData[3].color[2] = 1.;

    calcBBox (&sphereData[3]);

}



/* Draw each sphere, unless it is culled */

static int drawSphere (sphereStr sd)

{

    /* If sw culling is enabled (toggle with <space> key),

       test to see if the bounding box is outside the view volume.

       If so, just return. Otherwise, render the sphere. */

    if (swCull && culled(sd)) {

        return 1;

    }

    

    /* Not trivially culled */

    glPushMatrix ();

    glTranslatef (sd.trans[0], sd.trans[1], sd.trans[2]);

    glColor3f (sd.color[0], sd.color[1], sd.color[2]);

    gluSphere (quadSphere, sd.radius, 16, 8);

    glPopMatrix ();



    return 0;

}









/*

 * Main code

 */



/* Called both when the view changes and at init time. */

static void setViewTransform ()

{

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity ();

    gluLookAt (camera[0], camera[1], camera[2],

               lookat[0], lookat[1], lookat[2],

               0., 1., 0.);

}



/* Handle keyboard input */

#define DELTA 0.25f

static void key (unsigned char k, int x, int y)

{

    switch (k) {

    case 'q': lookat[0] -= DELTA; break;

    case 'Q': lookat[0] += DELTA; break;

    case 'w': lookat[1] -= DELTA; break;

    case 'W': lookat[1] += DELTA; break;

    case 'e': lookat[2] -= DELTA; break;

    case 'E': lookat[2] += DELTA; break;

    case 'a': camera[0] -= DELTA; break;

    case 'A': camera[0] += DELTA; break;

    case 's': camera[1] -= DELTA; break;

    case 'S': camera[1] += DELTA; break;

    case 'd': camera[2] -= DELTA; break;

    case 'D': camera[2] += DELTA; break;

        break;

    case ' ':

        swCull = !swCull;

        break;

    case '?':

        printf ("Move the look at point:\n");

        printf ("\tq/Q\talong the X axis;\n");

        printf ("\tw/W\talong the Y axis;\n");

        printf ("\te/E\talong the Z axis.\n");

        printf ("Move the camera position:\n");

        printf ("\ta/A\talong the X axis;\n");

        printf ("\ts/S\talong the Y axis;\n");

        printf ("\td/D\talong the Z axis.\n");

        printf ("<space> toggle sw culling\n");

        printf ("? displays help message\n");

        printf ("Any other key exits.\n");

        break;

    default:

        exit(0);

    }

    

    /* The view possibly changed. Set the view transform

       and calculate new clip planes */

    setViewTransform ();

    calcViewVolumePlanes ();

    

    glutPostRedisplay ();

}



static void display( void )

{

    int cullCount = 0;



    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    /* Establish light position */

    {

         GLfloat pos[4] = {3., 5., 2., 1.};

         glLightfv (GL_LIGHT1, GL_POSITION, pos);

    }



    /* Draw the lookat position */

    glColor3f (1., 1., 1.);

    glPushMatrix ();

    glTranslatef (lookat[0], lookat[1], lookat[2]);

    glDisable (GL_LIGHTING);

    glBegin (GL_LINES);

    glColor3f (1., 0., 0.);

    glVertex3f (-.25, 0., 0.);

    glVertex3f (.25, 0., 0.);

    glColor3f (1., 1., 0.);

    glVertex3f (0., -.25, 0.);

    glVertex3f (0., .25, 0.);

    glColor3f (0., 0., 1.);

    glVertex3f (0., 0., -.25);

    glVertex3f (0., 0., .25);

    glEnd ();

    glEnable (GL_LIGHTING);

    glPopMatrix ();



    /* Draw spheres, tested against the clip planes */

    cullCount += drawSphere (sphereData[0]);

    cullCount += drawSphere (sphereData[1]);

    cullCount += drawSphere (sphereData[2]);

    cullCount += drawSphere (sphereData[3]);



    printf("Culled %d, rendered %d\n",

           cullCount, 4-cullCount);



    glutSwapBuffers();

}



void reshape(int w, int h)

{

    glViewport (0, 0, w, h);       

    

    glMatrixMode (GL_PROJECTION);  

    glLoadIdentity ();

    gluPerspective (50., (float)w/(float)h, 5., 20.);

    

    /* New projection matrix.

       (If this is the first time through, the ModelView matrix

       is already valid -- it was set at init time.)

       Calculate clip planes. */

    calcViewVolumePlanes ();



    glMatrixMode(GL_MODELVIEW);

}



static void init ()

{

    /* Generate our geometry */

    initSpheres ();



    /* Initially, we're software culling (toggle with <space> key) */

    swCull = 1;

    

    /* Set up camera info and establish initial view */

    lookat[0] = lookat[1] = lookat[2] = 0.f;

    camera[0] = camera[1] = 0.; camera[2] = 10.f;

    setViewTransform ();



    glEnable (GL_DEPTH_TEST);



    {

        GLfloat white[4] = {1., 1., 1., 1.};

        GLfloat black[4] = {0., 0., 0., 0.};



        /* Set up light1 */

        glEnable (GL_LIGHTING);

        glEnable (GL_LIGHT1);

        glLightfv (GL_LIGHT1, GL_DIFFUSE, white);

        glLightfv (GL_LIGHT1, GL_SPECULAR, black);



        /* ambient and diffuse will track glColor */

        glEnable (GL_COLOR_MATERIAL);

        glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        glMaterialfv (GL_FRONT, GL_SPECULAR, black);

    }



    quadSphere = gluNewQuadric ();



    glutDisplayFunc (display); 

    glutReshapeFunc (reshape);

    glutKeyboardFunc (key);

}



int main(int argc, char** argv)
{

    glutInit (&argc,argv);

    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); 

    glutInitWindowSize (300,300);

    glutInitWindowPosition (0,0); 

    glutCreateWindow ("view volume culling");



    init ();



    glutMainLoop ();
    
    return 0;

}





