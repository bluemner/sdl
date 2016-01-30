// =================================================
// File : driver.c
// Author : Brandon Bluemner
// Description : Basic SDL 2.0 Implimentation
// LICENSE : MIT
// =================================================
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <string.h>


SDL_Window* WINDOW = NULL;
SDL_GLContext glContext;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main ( int argc, char * args[] );
int init ();
int initGL ();

void close ();
void events ( SDL_Event &event );
void resize ( int width, int height );
void redraw ();
bool KEEP_ALIVE = true;

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
				     
		WINDOW = SDL_CreateWindow( "SDL Lesson 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

// -------------------------------------------------------------
// Function called on redraw event
// -------------------------------------------------------------
void redraw ( )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		// Clear The Screen And The Depth Buffer
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();		
 
  glPushMatrix();
  

		glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
		// draw a triangle (in smooth coloring mode)
				
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
		
		glColor3f(0.5f,0.5f,1.0f);			// set color to a blue shade.
		
		glBegin( GL_QUADS );				// start drawing a polygon (4 sided)
			glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
			glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
			glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
			glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left	
		glEnd();					// done with the polygon
			
  glPopMatrix ( );
  
  SDL_GL_SwapWindow( WINDOW );
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
						printf("A Key Pressed\n");
					break;
					
					case SDLK_b:
						printf("B Key Pressed\n");
					break;
		
					case SDLK_c:
						printf("C Key Pressed\n");
					break;
					
					default:
						//Do nothing
					break;
				 }//end switch(event.key.keysym.sym)
            	 break;
				 
			case SDL_KEYUP:
				 //Todo Key up commands
				 break;
			
		  }//end switch(event.type)
		  
	}//end-while
}