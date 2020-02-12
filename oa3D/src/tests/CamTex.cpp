// Open CV includes
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

// Standard includes
#include <stdio.h>
#include <string.h>
#include <assert.h>

// OpenGL/Glut includes
#include <GL/glut.h>

// Timing includes
#include <sys/time.h>

#define KEY_ESCAPE 27


CvCapture* g_Capture;
GLint g_hWindow;

// Frame size
int frame_width  = 640;
int frame_height = 480;

// current frames per second, slightly smoothed over time
double fps;
// show mirror image
bool mirror = true;

// Return current time in seconds
double current_time_in_seconds();
// Initialize glut window
GLvoid init_glut();
// Glut display callback, draws a single rectangle using video buffer as
// texture
GLvoid display();
// Glut reshape callback
GLvoid reshape(GLint w, GLint h);
// Glut keyboard callback
GLvoid key_press (unsigned char key, GLint x, GLint y);
// Glut idle callback, fetches next video frame
GLvoid idle();

double current_time_in_seconds()
{
  struct timeval timer;
  gettimeofday(&timer,NULL);
  double seconds = 1e-6 * timer.tv_usec + timer.tv_sec;
  return seconds;
}


GLvoid init_glut()
{

  glClearColor (0.0, 0.0, 0.0, 0.0);

  // Set up callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key_press);
  glutIdleFunc(idle);
}

GLvoid display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  // These are necessary if using glTexImage2D instead of gluBuild2DMipmaps
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Set Projection Matrix
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, frame_width, frame_height, 0.0f, 0.0f, 1.0f);

  // Switch to Model View Matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Allegro-to-View Matrix


  // Draw a textured quad
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(frame_width, 0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(frame_width, frame_height);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, frame_height);
  glEnd();

  glFlush();
  glutSwapBuffers();
}


GLvoid reshape(GLint w, GLint h)
{
  glViewport(0, 0, w, h);
}

GLvoid key_press(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'f':
      printf("fps: %g\n",fps);
      break;
    case 'm':
      mirror = !mirror;
      break;
    case KEY_ESCAPE:
      //|cvReleaseCapture(&g_Capture);
      glutDestroyWindow(g_hWindow);
      exit(0);
      break;
  }
  glutPostRedisplay();
}


GLvoid idle()
{
  // start timer
  double start_seconds = current_time_in_seconds();

  // Capture next frame, this will almost always be the limiting factor in the
  // framerate, my webcam only gets ~15 fps
  IplImage * image = cvQueryFrame(g_Capture);

  // Of course there are faster ways to do this with just opengl but this is to
  // demonstrate filtering the video before making the texture
  if(mirror)
  {
    cvFlip(image, NULL, 1);
  }

  // Image is memory aligned which means we there may be extra space at the end
  // of each row. gluBuild2DMipmaps needs contiguous data, so we buffer it here
  char *buffer = new char[image->width*image->height*image->nChannels];
  int step     = image->widthStep;
  int height   = image->height;
  int width    = image->width;
  int channels = image->nChannels;
  char * data  = (char *)image->imageData;
  // memcpy version below seems slightly faster
  //for(int i=0;i<height;i++)
  //for(int j=0;j<width;j++)
  //for(int k=0;k<channels;k++)
  //{
  //  buffer[i*width*channels+j*channels+k] = data[i*step+j*channels+k];
  //}
  for(int i=0;i<height;i++)
  {
    memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
  }

  // Create Texture
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    image->width,
    image->height,
    0,
    GL_BGR,
    GL_UNSIGNED_BYTE,
    buffer);


  delete[] buffer;
  // Update display
  glutPostRedisplay();

  double stop_seconds = current_time_in_seconds();
  fps = 0.9*fps + 0.1*1.0/(stop_seconds-start_seconds);
}

int main(int argc, char* argv[])
{

  // Create OpenCV camera capture
  g_Capture = cvCaptureFromCAM(0);
  assert(g_Capture);
  // capture properties
  frame_height = (int)cvGetCaptureProperty(g_Capture, CV_CAP_PROP_FRAME_HEIGHT);
  frame_width  = (int)cvGetCaptureProperty(g_Capture, CV_CAP_PROP_FRAME_WIDTH);

  // Create GLUT Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(frame_width, frame_height);

  g_hWindow = glutCreateWindow("Video Texture");

  // Initialize OpenGL
  init_glut();

  glutMainLoop();

  return 0;
}
