/** MIT License

	The MIT License (MIT)
	Copyright (c) 2017 Brandon Bluemner

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
 */


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

SDL_Window * WINDOW = NULL;
SDL_GLContext glContext;

// Constents section
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* TITLE = "Graph";

// functions & global variabls

bool KEEP_ALIVE = true;

int main ( int argc, char * args[] );
int init ();
int initGL ();

void close ();
void events ( SDL_Event &event );
void resize ( int width, int height );
void redraw ();
void grid();


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
void redraw ( )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		// Clear The Screen And The Depth Buffer
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();		
	
	glPushMatrix();
	grid();
	
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

void resize ( int width, int height )
{
  glViewport(0, 0,  width, height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}
void close ( )
{
	SDL_DestroyWindow( WINDOW );
	WINDOW = NULL;
	SDL_Quit();
}

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