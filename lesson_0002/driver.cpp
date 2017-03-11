// =================================================
// File : driver.c
// Author : Brandon Bluemner
// Description : Basic SDL 2.0 Implimentation
// LICENSE : MIT
// =================================================


#ifdef _WIN32
	#define SDL_MAIN_HANDLED
	#include <Windows.h>
	#include <GLU.h>
	#include <GL/gl.h>
#elif linux

	#include <GL/glu.h>

#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string.h>
#include <string>

#define PI 3.14159265359

SDL_Window* WINDOW = NULL;
SDL_GLContext glContext;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* TITLE = "Lesson 02";
int main ( int argc, char * args[] );
int init ();
int initGL ();

void close ();
void events ( SDL_Event &event );
void resize ( int width, int height );
void redraw ();
void grid();
bool KEEP_ALIVE = true;
bool SHOW_GRID = false;
bool SHIFT = false;
//camera location in longitude and latitude
static float alpha = 0.0;
static float beta = PI / 6.0;
static float zoom = 10;

// position of camera 
static GLdouble cpos[3];

// position of light source
static GLfloat lpos[] = { -2.5, 2.5, 3.0, 1.0 };


static GLfloat black[]     = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat white[]     = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat gray[]      = { 0.5, 0.5, 0.5, 1.0 };

static GLfloat red[]       = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat green[]     = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat blue[]      = { 0.0, 0.0, 1.0, 1.0 };

static GLfloat yellow[]    = { 1.0, 1.0, 0.0, 1.0 };
static GLfloat magenta[]   = { 1.0, 0.0, 1.0, 1.0 };
static GLfloat cyan[]      = { 0.0, 1.0, 1.0, 1.0 };

static GLfloat darkcyan[]  = { 0.0, 0.4, 0.4, 1.0 };
static GLfloat orange[]    = { 255.0, 102, 0, 1.0 };


int main ( int argc, char * args[] )
{
	if( init () )
	{
		printf("Error loading init\n");
		return -1;
	}	
	SDL_Event event;
			
	while( KEEP_ALIVE )
	{
		//Handle Events
		events ( event );
	    redraw ();
	}
	close ();
	return 0;	
}

int init()
{
	int result = 0;
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "Problem loading SLD Error: %s\n", SDL_GetError() );
	}
	else
	{
		//OPEN GL VERSION 2.X
		SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		//OPEN GL VERSION ^.1
		SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
				
		//Create window
		   int  videoFlags  = SDL_WINDOW_OPENGL;
				videoFlags |= SDL_WINDOW_SHOWN; 		    
				videoFlags |= SDL_WINDOW_RESIZABLE;  
				     
		WINDOW = SDL_CreateWindow( TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		                           SCREEN_WIDTH, SCREEN_HEIGHT, videoFlags );
		if( WINDOW == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			result = -1;
		}
		else
		{
			glContext = SDL_GL_CreateContext ( WINDOW );			
			
			if( glContext == NULL )
			{
				printf( "Unable to crate Open GL Context: %s\n", SDL_GetError() );
				result = -2;
			}
			else
			{
				//Try Vsync 
				if( SDL_GL_SetSwapInterval ( 1 )  < 0 )
				{
					printf ( "Vsync error:%s\n", SDL_GetError() );
				}
				
				if( initGL () )
				{
					printf ( "Problem working with OpenGL" );
				}
			}

		}
	}
	
	return result;
}

// ------------------------------------
// OpenGL Initialization Function
// ------------------------------------
int initGL ()
{
	int result = 0; 
	
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );		// This Will Clear The Background Color To Black
    glClearDepth ( 1.0 );				// Enables Clearing Of The Depth Buffer
	glDepthFunc ( GL_LESS );	
				
    glEnable ( GL_DEPTH_TEST );			// Enables Depth Testing
    glShadeModel ( GL_SMOOTH );			// Enables Smooth Color Shading
	glMatrixMode ( GL_PROJECTION );
	  /* Really Nice Perspective Calculations */
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	//
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//
	
	glLoadIdentity ();	
	GLenum error = glGetError ();
	
	if(  error != GL_NO_ERROR )
	{
		printf ( "OpenGL Error %s\n", gluErrorString( error ) );
		result = -1;
		return result;
	}
	
	glMatrixMode (GL_MODELVIEW);
	
    // Set up the view port
	glViewport (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (45.0f,(GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT,0.1f,100.0f);
    glMatrixMode (GL_MODELVIEW);
		
	return result;
}

// -------------------------------------------------------------
// Function called on resize event
// width : with of the window
// height : height of the window
// -------------------------------------------------------------
void resize ( int width, int height )
{
  glViewport(0, 0,  width, height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);

}
void cube ()
{
  glBegin(GL_QUADS);		// start drawing the cube.  
		// top of cube
		
		glVertex3f( 1.0f, 1.0f,-1.0f);		// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);		// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);		// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);		// Bottom Right Of The Quad (Top)
		
		// bottom of cube

		glVertex3f( 1.0f,-1.0f, 1.0f);		// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);		// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);		// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);		// Bottom Right Of The Quad (Bottom)
		
		// front of cube
		
		glVertex3f( 1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);		// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);		// Bottom Right Of The Quad (Front)
		
		// back of cube.
	
		glVertex3f( 1.0f,-1.0f,-1.0f);		// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);		// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);		// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);		// Bottom Right Of The Quad (Back)
		
		// left of cube
	
		glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);		// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);		// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);		// Bottom Right Of The Quad (Left)
		
		// Right of cube
		glVertex3f( 1.0f, 1.0f,-1.0f);	        // Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);		// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);		// Bottom Right Of The Quad (Right)
  glEnd();					// Done Drawing The Cube
}
// -------------------------------------------------------------
// Function called on redraw event
// -------------------------------------------------------------
void redraw ( )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		// Clear The Screen And The Depth Buffer
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();		
 
  glPushMatrix();
		/* update camera position */
		cpos[0] = zoom * cos(beta) * sin(alpha);
		cpos[1] = zoom * sin(beta);
		cpos[2] = zoom * cos(beta) * cos(alpha);
		gluLookAt(cpos[0], cpos[1], cpos[2], 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
	
		/* set current light source position */
		glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	
		/* draw sphere to show light source */
		glMaterialfv(GL_FRONT, GL_EMISSION, white);
		glPushMatrix();
			glTranslatef(lpos[0], lpos[1], lpos[2]);
			cube();				
  		glPopMatrix();
 	//remaining objects do not look as if they emit light
 	//glMaterialfv(GL_FRONT, GL_EMISSION, black);
	if( SHOW_GRID )
	{
		grid();		  
	}
	glPushMatrix( );
	
	
			glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
			// draw a triangle (in smooth coloring mode)
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);	
			glNormal3f(0,0,-1);	
			glBegin( GL_POLYGON );				// start drawing a polygon
				glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
				glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
				glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
				glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
				glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
				glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left	
			glEnd();					// we're done with the polygon (smooth color interpolation)	
			
			glTranslatef(3.0f,0.0f,0.0f);		        // Move Right 3 Units		
			// draw a square (quadrilateral)
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
			glColor3f(0.5f,0.5f,1.0f);			// set color to a blue shade.
			
			
			glBegin( GL_QUADS );				// start drawing a polygon (4 sided)
				glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
				glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
				glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
				glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left	
			glEnd();					// done with the polygon
				
	glPopMatrix ( );
	
  glPopMatrix ( );
  SDL_GL_SwapWindow( WINDOW );
}

void grid()
{
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,gray);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
	glTranslatef(0.0f,0.0f,0.0f);	
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);

	/*grid*/
	float GRIDE_SIZE = 100;
	//X
	glBegin(GL_LINES);
		glVertex2f(-GRIDE_SIZE, 0);
		glVertex2f(GRIDE_SIZE, 0);
	glEnd();
	//Y
	glBegin(GL_LINES);
		glVertex2f(0, -GRIDE_SIZE);
		glVertex2f(0, GRIDE_SIZE);
	glEnd();
	//Z
	glBegin(GL_LINES);
		glVertex3f(0, 0, -GRIDE_SIZE);
		glVertex3f(0, 0, GRIDE_SIZE);
	glEnd();
	
	//X ticks
	float TICK_SIZE = .5;
	for (float f = -GRIDE_SIZE; f <= GRIDE_SIZE; f += 1)
	{
		//X
		glBegin(GL_LINES);
			glVertex3f(f, -TICK_SIZE,0);
			glVertex3f(f, TICK_SIZE,0);
		glEnd();		
		//Y
		glBegin(GL_LINES);
			glVertex2f(-TICK_SIZE, f);
			glVertex2f(TICK_SIZE, f);
		glEnd();
		//Z
		glBegin(GL_LINES);
			glVertex3f(0, -TICK_SIZE, f);
			glVertex3f(0, TICK_SIZE, f);
		glEnd();
	}

	
}



// -------------------------------------------------------------
// Function called on close event
// -------------------------------------------------------------
void close ( )
{
	SDL_DestroyWindow( WINDOW );	
	WINDOW = NULL;
	SDL_Quit();
}
// -------------------------------------------------------------
// Function to handel events
// &event  : SDL_Event pointer to instance of event
// -------------------------------------------------------------
void events( SDL_Event &event )
{
	while( SDL_PollEvent( &event ) != 0 )
	{	
		  //https://wiki.libsdl.org/SDL_EventType
		  switch(event.type)
		  {
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						//resize();
            			break;
       	    		case SDL_WINDOWEVENT_SIZE_CHANGED:					     
			    		resize(event.window.data1,event.window.data2 );
						break;	
				}
				break;
			//user-requested quit
            case SDL_QUIT:
				 KEEP_ALIVE = false;
				 break;
			//Mouse events
			case SDL_MOUSEMOTION:
				 SDL_MOUSEBUTTONDOWN:
				 SDL_MOUSEBUTTONUP:
				 SDL_MOUSEWHEEL:
            break;			
		
			//Keyboard Events
            case SDL_KEYDOWN:
                 switch( event.key.keysym.sym )
				 {						
					case SDLK_ESCAPE:
						KEEP_ALIVE = false;
					break;
					
					case SDLK_LSHIFT:
						SHIFT = true;
					break;
					
					case SDLK_UP:
						printf("UP Key Pressed\n");
					break;
		
					case SDLK_DOWN:
						printf("Down Key Pressed\n");
					break;
		
					case SDLK_LEFT:
						printf("Left Key Pressed\n");
					break;
		
					case SDLK_RIGHT:
						printf("Right Key Pressed\n");
					break;
					
					case SDLK_a:					
						
					break;
					
					
					case SDLK_b:
						printf("B Key Pressed\n");
					break;
		
					case SDLK_c:
						printf("C Key Pressed\n");
					break;
					
					case SDLK_e:
						lpos[0] = (SHIFT) ? lpos[0] - 0.2 : lpos[0] + 0.2;
						redraw();
					break;
					
					case SDLK_g:
						SHOW_GRID = !SHOW_GRID;
					break;
					
					case SDLK_r:
						lpos[1] = (SHIFT) ? lpos[1] - 0.2 : lpos[1] + 0.2;
						redraw();
					break;
					
					case SDLK_s:						
						zoom += 0.5;
						if (zoom < 0)
						{ 
							zoom = 0;
						}
						redraw();
					break;
					
					case SDLK_t:
						lpos[2] = (SHIFT) ? lpos[2] - 0.2 : lpos[2] + 0.2;
						redraw();
					break;
					
					case SDLK_w:						
						zoom -= 0.5;
						if (zoom > 60)
						{ 
						   zoom = 20;
						}
						redraw();
					break;
					
					default:
						//Do nothing
					break;
				 }//end switch(event.key.keysym.sym)
            	 break;
				 
			case SDL_KEYUP:
				 //Todo Key up commands
				 switch( event.key.keysym.sym )
				 {
					 case SDLK_LSHIFT:
						SHIFT = false;
					break;
				 }
				 break;
			
		  }//end switch(event.type)
		  
	}//end-while
}